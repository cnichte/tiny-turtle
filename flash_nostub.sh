#!/bin/bash
# Flash-Script für ESP32-C6 mit --no-stub Option
# Umgeht Checksum-Probleme beim Stub-Upload
# 
# WICHTIG: ESP32-C6 hat zwei USB-Ports:
# - USB-JTAG Port (normalerweise ...58131 oder ...343401)
# - USB-UART Port (normalerweise ...626461 oder ...627361) <- DIESEN VERWENDEN!
#
# Verwendung:
#   ./flash_nostub.sh [PORT] [BAUDRATE] [PROJECT_NAME]
#   Beispiel: ./flash_nostub.sh /dev/cu.usbmodem59720627361 115200 robart-firmware-esp32

# Automatische Port-Erkennung
if [ -z "$1" ]; then
    # Suche nach USB-Ports, bevorzuge die mit höheren Nummern (UART-Port)
    AVAILABLE_PORTS=$(ls /dev/cu.usbmodem* 2>/dev/null | sort -r)
    if [ -z "$AVAILABLE_PORTS" ]; then
        echo "FEHLER: Keine USB-Ports gefunden!"
        echo "Bitte Board anschließen und erneut versuchen."
        exit 1
    fi
    
    echo "Verfügbare USB-Ports:"
    echo "$AVAILABLE_PORTS"
    echo ""
    
    # Nimm den ersten Port (höchste Nummer = meist UART)
    PORT=$(echo "$AVAILABLE_PORTS" | head -1)
    echo "Auto-Auswahl: $PORT (USB-UART Port empfohlen)"
    echo "Falls das nicht funktioniert, versuche den anderen Port!"
    echo ""
else
    PORT="$1"
fi

BAUD="${2:-115200}"

# Automatische Projekt-Namen-Erkennung
if [ -z "$3" ]; then
    # Versuche aus CMakeLists.txt zu lesen
    if [ -f "CMakeLists.txt" ]; then
        PROJECT_NAME=$(grep -m1 "project(" CMakeLists.txt | sed 's/project(\(.*\))/\1/' | tr -d ' )')
    fi
    
    # Falls nicht gefunden, verwende Verzeichnisnamen
    if [ -z "$PROJECT_NAME" ]; then
        PROJECT_NAME=$(basename "$PWD")
    fi
else
    PROJECT_NAME="$3"
fi

echo "========================================"
echo "ESP32-C6 Flash Script (NO-STUB Mode)"
echo "========================================"
echo "Port:         $PORT"
echo "Baudrate:     $BAUD"
echo "Projekt:      $PROJECT_NAME"
echo ""
echo "TIPP: Wenn Flash fehlschlägt:"
echo "  1. Anderen USB-Port am ESP32-C6 verwenden (USB-UART statt USB-JTAG)"
echo "  2. BOOT-Taste drücken + RESET, dann BOOT loslassen"
echo "  3. Anderes USB-Kabel oder Hub versuchen"
echo "========================================"
echo ""

# Prüfe ob build-Verzeichnis existiert
if [ ! -d "build" ]; then
    echo "FEHLER: build/ Verzeichnis nicht gefunden!"
    echo "Bitte zuerst 'idf.py build' ausführen."
    exit 1
fi

# Prüfe ob Binary existiert
if [ ! -f "build/${PROJECT_NAME}.bin" ]; then
    echo "FEHLER: build/${PROJECT_NAME}.bin nicht gefunden!"
    echo "Gefundene .bin Dateien:"
    ls -la build/*.bin 2>/dev/null || echo "  Keine .bin Dateien gefunden"
    exit 1
fi

echo "Starte Flash-Vorgang..."
echo ""

python3 -m esptool \
  --chip esp32c6 \
  -p "$PORT" \
  -b "$BAUD" \
  --before default_reset \
  --after hard_reset \
  --no-stub \
  write_flash \
  --flash_mode dio \
  --flash_freq 80m \
  --flash_size 2MB \
  0x0 build/bootloader/bootloader.bin \
  0x10000 "build/${PROJECT_NAME}.bin" \
  0x8000 build/partition_table/partition-table.bin

RESULT=$?

echo ""
if [ $RESULT -eq 0 ]; then
    echo "========================================"
    echo "✓ Flash erfolgreich abgeschlossen!"
    echo "========================================"
    echo ""
    echo "Monitor starten mit:"
    echo "  idf.py -p $PORT monitor"
else
    echo "========================================"
    echo "✗ Flash fehlgeschlagen (Exit Code: $RESULT)"
    echo "========================================"
    echo ""
    echo "Mögliche Lösungen:"
    echo "  1. USB-UART Port verwenden (anderen Port probieren)"
    echo "  2. Board manuell in Bootloader-Modus versetzen:"
    echo "     - BOOT-Taste gedrückt halten"
    echo "     - RESET-Taste kurz drücken"
    echo "     - BOOT-Taste loslassen"
    echo "     - Script erneut ausführen"
    echo "  3. Anderes USB-Kabel testen"
    echo "  4. Baudrate reduzieren: ./flash_nostub.sh $PORT 57600"
fi

exit $RESULT
