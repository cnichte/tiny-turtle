/**
 * @file neopixel.cpp
 * @brief WS2812B NeoPixel LED-Treiber Implementierung
 *
 * Verwendet das ESP32 RMT-Peripheral für das präzise WS2812-Timing.
 *
 * WS2812 Timing (bei 10MHz RMT-Clock = 100ns pro Tick):
 * - T0H: 0.4µs (4 Ticks high)
 * - T0L: 0.85µs (8-9 Ticks low)
 * - T1H: 0.8µs (8 Ticks high)
 * - T1L: 0.45µs (4-5 Ticks low)
 * - Reset: >50µs low
 */

#include "neopixel.h"

#include <algorithm>
#include <cstring>

#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace
{
    constexpr const char *TAG = "neopixel";

    // WS2812 Timing in RMT-Ticks (bei 10MHz = 100ns/Tick)
    constexpr uint16_t WS2812_T0H_TICKS = 4;     // 0.4µs
    constexpr uint16_t WS2812_T0L_TICKS = 8;     // 0.8µs
    constexpr uint16_t WS2812_T1H_TICKS = 8;     // 0.8µs
    constexpr uint16_t WS2812_T1L_TICKS = 4;     // 0.4µs
    constexpr uint16_t WS2812_RESET_TICKS = 500; // 50µs Reset

    // RMT-Auflösung in Hz (10MHz = 100ns pro Tick)
    constexpr uint32_t RMT_RESOLUTION_HZ = 10000000;

    /**
     * @brief Benutzerdefinierter RMT-Encoder für WS2812
     */
    typedef struct
    {
        rmt_encoder_t base;
        rmt_encoder_t *bytes_encoder;
        rmt_encoder_t *copy_encoder;
        rmt_symbol_word_t reset_code;
        int state;
    } ws2812_encoder_t;

    /**
     * @brief Encoder-Callback für WS2812
     */
    static size_t ws2812_encode(rmt_encoder_t *encoder, rmt_channel_handle_t channel,
                                const void *primary_data, size_t data_size,
                                rmt_encode_state_t *ret_state)
    {
        ws2812_encoder_t *ws2812_encoder = __containerof(encoder, ws2812_encoder_t, base);
        rmt_encode_state_t session_state = RMT_ENCODING_RESET;
        size_t encoded_symbols = 0;

        switch (ws2812_encoder->state)
        {
        case 0: // Sende Pixel-Daten
            encoded_symbols = ws2812_encoder->bytes_encoder->encode(
                ws2812_encoder->bytes_encoder, channel, primary_data, data_size, &session_state);
            if (session_state & RMT_ENCODING_COMPLETE)
            {
                ws2812_encoder->state = 1; // Weiter zu Reset
            }
            if (session_state & RMT_ENCODING_MEM_FULL)
            {
                *ret_state = static_cast<rmt_encode_state_t>(session_state);
                return encoded_symbols;
            }
            [[fallthrough]];

        case 1: // Sende Reset-Signal
            encoded_symbols += ws2812_encoder->copy_encoder->encode(
                ws2812_encoder->copy_encoder, channel, &ws2812_encoder->reset_code,
                sizeof(ws2812_encoder->reset_code), &session_state);
            if (session_state & RMT_ENCODING_COMPLETE)
            {
                ws2812_encoder->state = 0; // Zurück zum Anfang
                *ret_state = RMT_ENCODING_COMPLETE;
            }
            if (session_state & RMT_ENCODING_MEM_FULL)
            {
                *ret_state = static_cast<rmt_encode_state_t>(session_state);
            }
            break;
        }
        return encoded_symbols;
    }

    static esp_err_t ws2812_encoder_reset(rmt_encoder_t *encoder)
    {
        ws2812_encoder_t *ws2812_encoder = __containerof(encoder, ws2812_encoder_t, base);
        rmt_encoder_reset(ws2812_encoder->bytes_encoder);
        rmt_encoder_reset(ws2812_encoder->copy_encoder);
        ws2812_encoder->state = 0;
        return ESP_OK;
    }

    static esp_err_t ws2812_encoder_delete(rmt_encoder_t *encoder)
    {
        ws2812_encoder_t *ws2812_encoder = __containerof(encoder, ws2812_encoder_t, base);
        rmt_del_encoder(ws2812_encoder->bytes_encoder);
        rmt_del_encoder(ws2812_encoder->copy_encoder);
        delete ws2812_encoder;
        return ESP_OK;
    }

    /**
     * @brief Erstellt einen WS2812 RMT-Encoder
     */
    static esp_err_t create_ws2812_encoder(rmt_encoder_handle_t *ret_encoder)
    {
        ws2812_encoder_t *ws2812_encoder = new ws2812_encoder_t();
        if (!ws2812_encoder)
        {
            return ESP_ERR_NO_MEM;
        }

        ws2812_encoder->base.encode = ws2812_encode;
        ws2812_encoder->base.reset = ws2812_encoder_reset;
        ws2812_encoder->base.del = ws2812_encoder_delete;
        ws2812_encoder->state = 0;

        // Bytes-Encoder für die Pixel-Daten
        rmt_bytes_encoder_config_t bytes_encoder_config = {
            .bit0 = {
                .duration0 = WS2812_T0H_TICKS,
                .level0 = 1,
                .duration1 = WS2812_T0L_TICKS,
                .level1 = 0,
            },
            .bit1 = {
                .duration0 = WS2812_T1H_TICKS,
                .level0 = 1,
                .duration1 = WS2812_T1L_TICKS,
                .level1 = 0,
            },
            .flags = {
                .msb_first = 1, // WS2812 erwartet MSB zuerst
            },
        };

        esp_err_t ret = rmt_new_bytes_encoder(&bytes_encoder_config, &ws2812_encoder->bytes_encoder);
        if (ret != ESP_OK)
        {
            delete ws2812_encoder;
            return ret;
        }

        // Copy-Encoder für das Reset-Signal
        rmt_copy_encoder_config_t copy_encoder_config = {};
        ret = rmt_new_copy_encoder(&copy_encoder_config, &ws2812_encoder->copy_encoder);
        if (ret != ESP_OK)
        {
            rmt_del_encoder(ws2812_encoder->bytes_encoder);
            delete ws2812_encoder;
            return ret;
        }

        // Reset-Signal: Low für >50µs
        ws2812_encoder->reset_code = {
            .duration0 = WS2812_RESET_TICKS,
            .level0 = 0,
            .duration1 = 0,
            .level1 = 0,
        };

        *ret_encoder = &ws2812_encoder->base;
        return ESP_OK;
    }

} // namespace

NeoPixel::NeoPixel(int pin, int pixels)
    : pin_(pin), count_(pixels), buffer_(pixels * 3, 0) {}

NeoPixel::~NeoPixel()
{
    if (rmt_encoder_)
    {
        rmt_del_encoder(rmt_encoder_);
    }
    if (rmt_channel_)
    {
        rmt_disable(rmt_channel_);
        rmt_del_channel(rmt_channel_);
    }
}

esp_err_t NeoPixel::begin()
{
    if (started_)
    {
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Initialisiere NeoPixel auf GPIO %d mit %d LEDs", pin_, count_);

    // RMT TX-Kanal erstellen
    rmt_tx_channel_config_t tx_config = {
        .gpio_num = static_cast<gpio_num_t>(pin_),
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = RMT_RESOLUTION_HZ,
        .mem_block_symbols = 64,
        .trans_queue_depth = 4,
        .intr_priority = 0,
        .flags = {
            .invert_out = 0,
            .with_dma = 0,
            .io_loop_back = 0,
            .io_od_mode = 0,
            .allow_pd = 0,
        },
    };

    esp_err_t ret = rmt_new_tx_channel(&tx_config, &rmt_channel_);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Fehler beim Erstellen des RMT-Kanals: %s", esp_err_to_name(ret));
        return ret;
    }

    // WS2812 Encoder erstellen
    ret = create_ws2812_encoder(&rmt_encoder_);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Fehler beim Erstellen des WS2812-Encoders: %s", esp_err_to_name(ret));
        rmt_del_channel(rmt_channel_);
        rmt_channel_ = nullptr;
        return ret;
    }

    // Kanal aktivieren
    ret = rmt_enable(rmt_channel_);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Fehler beim Aktivieren des RMT-Kanals: %s", esp_err_to_name(ret));
        rmt_del_encoder(rmt_encoder_);
        rmt_del_channel(rmt_channel_);
        rmt_encoder_ = nullptr;
        rmt_channel_ = nullptr;
        return ret;
    }

    started_ = true;
    ESP_LOGI(TAG, "NeoPixel initialisiert");

    // Alle LEDs aus
    clear();
    show();

    return ESP_OK;
}

void NeoPixel::ensure_started()
{
    if (!started_)
    {
        begin();
    }
}

void NeoPixel::clear()
{
    std::fill(buffer_.begin(), buffer_.end(), 0);
}

uint32_t NeoPixel::Color(uint8_t r, uint8_t g, uint8_t b) const
{
    return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | b;
}

void NeoPixel::setPixelColor(int idx, uint32_t color)
{
    if (idx < 0 || idx >= count_)
        return;

    // WS2812 erwartet GRB-Reihenfolge!
    buffer_[idx * 3 + 0] = (color >> 8) & 0xFF;  // G
    buffer_[idx * 3 + 1] = (color >> 16) & 0xFF; // R
    buffer_[idx * 3 + 2] = color & 0xFF;         // B
}

void NeoPixel::show()
{
    ensure_started();

    if (!rmt_channel_ || !rmt_encoder_)
    {
        ESP_LOGW(TAG, "RMT nicht initialisiert");
        return;
    }

    rmt_transmit_config_t tx_config = {
        .loop_count = 0, // Kein Loop
        .flags = {
            .eot_level = 0,
            .queue_nonblocking = 0,
        },
    };

    esp_err_t ret = rmt_transmit(rmt_channel_, rmt_encoder_, buffer_.data(),
                                 buffer_.size(), &tx_config);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Fehler beim Senden: %s", esp_err_to_name(ret));
        return;
    }

    // Warten bis Übertragung abgeschlossen
    ret = rmt_tx_wait_all_done(rmt_channel_, pdMS_TO_TICKS(100));
    if (ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Timeout beim Warten auf TX: %s", esp_err_to_name(ret));
    }
}
