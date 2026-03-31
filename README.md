# STM32 GPS Logger
GPS data logger built on STM32F429I-DISC1 with NEO-6M GPS module. Logs real-time GPS fixes over serial, records them to CSV, and generates a live-updating map track.

## Hardware
- STM32F429I-DISC1
- NEO-6M GPS module
- CP2102 USB-UART adapter

## Wiring
- NEO-6M TX → STM32 PD2 (UART5 RX)
- STM32 PA9 (USART1 TX) → CP2102 RXD

## Firmware
Built with STM32CubeIDE using FreeRTOS. Three tasks handle GPS acquisition, NMEA parsing, and serial logging. GPS data is captured via DMA on UART5 and parsed from RMC sentences.

## Python Logger
Reads serial output from the STM32, logs fixes to `track.csv`, and generates a Folium map saved as `track.html`.

Install dependencies:
```bash
pip install pyserial folium
```

Confirm your serial port and update logger accordingly:
```bash
ls /dev/tty.*
```

Run the logger:
```bash
python3 gps_logger.py
```

The script will then print fixes to the terminal as they arrive in the format:
```
UTC: 023541.00  Lat: 40.785091  Lon: -73.968285  Speed: 0.59 kts
```

Output is also available directly via:
```bash
screen /dev/tty.usbserial-0001 115200
```

## Viewing the Map
Open `track.html` in any browser after running the logger. Refresh the page to see the route update as new fixes stream in!


Here's an example of it tracking a walk down the block:
![GPS Track](assets/map_visualiser.png)
