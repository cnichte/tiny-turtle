# README.md

## Tiny-Turtle Firmware für ESP-32

- <https://www.niklasroy.com/robotfactory/>

## Unterschiede zur Original Arduino-Firmware

Diese Firmware ist eine komplette Neuentwicklung für den ESP32-C6 mit ESP-IDF - ohne Arduino Framework.

### Hardware-Plattform

| Aspekt | Arduino (Original) | ESP32-C6 (Neu) |
|--------|-------------------|----------------|
| MCU | ATmega328P (8-bit AVR) | ESP32-C6 (32-bit RISC-V) |
| Takt | 16 MHz | 160 MHz |
| RAM | 2 KB | 512 KB |
| Flash | 32 KB | 4 MB |
| Cores | 1 | 1 (aber mit FreeRTOS) |

### Motor-Steuerung

| Arduino | ESP32-C6 |
|---------|----------|
| `delay()` blockiert CPU | **GPTimer mit ISR** - nicht-blockierend |
| Feste Schrittgeschwindigkeit | **Rampen** mit Beschleunigung/Abbremsung |
| Einfache Schleife | Hardware-Timer mit 1µs Auflösung |

### Multitasking

| Arduino | ESP32-C6 |
|---------|----------|
| Single-Threaded, alles sequentiell | **FreeRTOS Tasks** |
| `delay()` blockiert alles | `vTaskDelay()` gibt CPU frei |
| Kein Watchdog-Management | Task Watchdog integriert |

### NeoPixel LED

| Arduino | ESP32-C6 |
|---------|----------|
| Adafruit NeoPixel Library | **RMT-Peripheral** (Hardware-Timing) |
| Software-Bitbanging | Präzises Hardware-Timing ohne CPU-Last |

### Servo (PWM)

| Arduino | ESP32-C6 |
|---------|----------|
| Arduino Servo Library | **LEDC-Peripheral** (Hardware-PWM) |
| Timer-basiert | Dedizierte PWM-Hardware |

### Code-Struktur

| Arduino | ESP32-C6 |
|---------|----------|
| Einzelne `.ino` Datei | **Modulare Struktur** mit Namespaces |
| Globale Funktionen | `tiny_turtle::hal`, `tiny_turtle::motion`, etc. |
| Arduino Framework | **ESP-IDF** (natives SDK) |

### Timing-Funktionen

| Arduino | ESP32-C6 |
|---------|----------|
| `delay()`, `millis()` | Wrapper in `gpio_hal.cpp` |
| `analogRead()` | ESP-IDF ADC oneshot API |
| `tone()` | LEDC für Frequenzerzeugung |

### Vorteile der neuen Implementierung

- **10x schnellere CPU** für komplexere Berechnungen
- **Nicht-blockierende Motorsteuerung** durch Hardware-Timer
- **Sanfte Beschleunigung/Abbremsung** für präzisere Bewegungen
- **Zuverlässiges LED-Timing** durch RMT-Hardware
- **Saubere Code-Struktur** für einfache Erweiterung
- **WiFi/BLE verfügbar** für das was kommt...

## Projekt-Struktur

```txt
main/
├── tiny_turtle_app.cpp          (Entry Point)
├── CMakeLists.txt
└── tiny_turtle/
    ├── core/                    # Kern-Funktionalität
    │   ├── config.h             # Pin-Definitionen, Konstanten
    │   ├── types.h              # Gemeinsame Typen, Enums
    │   └── globals.cpp/.h       # Globale Variablen
    │
    ├── hal/                     # Hardware Abstraction Layer
    │   ├── gpio_hal.cpp/.h      # GPIO-Wrapper (pinMode, digitalWrite, delay)
    │   ├── neopixel.cpp/.h      # NeoPixel LED-Streifen
    │   ├── stepper.cpp/.h       # Schrittmotor-Steuerung (GPTimer)
    │   ├── servo.cpp/.h         # Servo (Pen up/down)
    │   ├── sensors.cpp/.h       # Bumper-Sensoren
    │   ├── audio.cpp/.h         # Speaker/Buzzer
    │   └── led.cpp/.h           # LED-Farben
    │
    ├── motion/                  # Bewegungs-Logik
    │   ├── motion.cpp/.h        # forward, backward, turn, move
    │   ├── spiral.cpp/.h        # Spiral-Bewegungen
    │   └── coordinates.cpp/.h   # Koordinaten-System
    │
    ├── drawing/                 # Zeichen-Funktionen
    │   ├── text.cpp/.h          # plotText, plotChar
    │   └── fonts.cpp/.h         # Font-Daten
    │
    ├── math/                    # Mathematische Funktionen
    │   └── trigonometry.cpp/.h  # Winkel- und Distanzberechnungen
    │
    ├── tiny_turtle.cpp          # Initialisierung
    └── tiny_turtle.h            # Public API (alles exportieren)
```

## Neopixel

Die NeoPixel LED (mit WS2812-Treiber) wird als Status-Anzeige verwendet. Die einzelne RGB-LED zeigt visuell den aktuellen Zustand des Roboters an - ob er bereit ist, ob der Stift oben/unten ist, oder spielt gerade Musik ab:

|Stelle               | Farbe                | Bedeutung               |
|---------------------|----------------------|-------------------------|
| tiny_turtle.cpp:30  | 🟢 Grün (0,32,0)     | System initialisiert.   |
| servo.cpp:27.       | ⚪ Dunkel (15,15,15) | Stift oben (penUp)      |
| servo.cpp:41        | ⚪ Hell (50,50,50)   | Stift unten (penDown)   |
| audio.cpp:36        | 🌈 Zufällig          | Lichtshow während Musik |

- <https://www.berrybase.de/sensoren-module/led/ws2812-13-neopixel/einzel-leds/>

## Welche USB Ports?

```bash
# Check PORTs
ls /dev/cu.*
```

## flash_nostub.sh

- Das ist mein spezielles Flash-Script für den ESP32-C6 mit --no-stub Option
- Umgeht Checksum-Probleme beim Stub-Upload.
- WICHTIG: Der ESP32-C6 hat zwei USB-Ports !!!
  - USB-JTAG Port (normalerweise ...58131 oder ...343401)
  - USB-UART Port (normalerweise ...626461 oder ...627361) <- DIESEN VERWENDEN!

Verwendung:

`./flash_nostub.sh [PORT] [BAUDRATE] [PROJECT_NAME]`

Beispiel:

```bash
./flash_nostub.sh /dev/cu.usbmodem59720627361 115200 HELLO_WORLD

./flash_nostub.sh /dev/cu.usbmodem5AAF2844941 115200 <<<<--- tiny turtle

# Monitor starten
source ~/.espressif/python_env/idf5.4_py3.14_env/bin/activate && idf.py -p /dev/cu.usbmodem5AAF2844941 monitor

Ctrl+] → Monitor beenden
Ctrl+T, Ctrl+H → Hilfe anzeigen
Ctrl+T, Ctrl+R → Reset des Boards
```

- Falls es nicht klappt:
  - `BOOT-Taste` drücken + `RESET`, dann `BOOT` loslassen, dann flott flashen.
  - Weitere Infos im `flash_nostub.sh`
