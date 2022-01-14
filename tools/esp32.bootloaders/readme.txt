To upload the firmware to ESP32 you need to upload following files to ESP32 to specyfic addresses:
- bootloader_dout_40m.bin: 0x1000
- boot_app0.bin: 0xe00
- partitions.bin: 0x8000
- AFE ESP32 firmware file: 0x10000

Upload using SPI_MODE = DOUT
Update the latest version or another AFE Firmware version can be made via web browser from AFE Configuration panel

------------------------------------------------

# Instrukcja PL

AFE Firmware do ESP32 należy załadować wgrywając wylistowane pliki pod konkretne adresy
- bootloader_dout_40m.bin: 0x1000
- boot_app0.bin: 0xe00
- partitions.bin: 0x8000
- AFE ESP32 firmware: 0x10000

Załaduj używając SPI_MODE = DOUT
Aktualizacja do nowszej wersji może zostać zrobiona za pomocą Panelu Konfiguracyjnego AFE Firmware