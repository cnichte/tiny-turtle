# README.md

Tiny-Turtle OS für ESP32.

FreeTos ordentlich genutzt?
https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/freertos.html

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
