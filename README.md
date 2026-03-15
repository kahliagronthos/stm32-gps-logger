# STM32 GPS Logger

GPS data logger built on STM32F429I-DISC1 with NEO-6M GPS module.

## Hardware
- STM32F429I-DISC1 (Cortex-M4 @ 180MHz)
- NEO-6M GPS module
- CP2102 USB-UART adapter

## Wiring
GPS TX --> PD2 (UART5 RX) --> NEO-6M TX 
Debug TX --> PA9 (USART1 TX) --> CP2102 RXD

## Phases
- [x] Phase 1 — Polling UART, raw NMEA output verified
- [x] Phase 2 — DMA circular mode, UART IDLE interrupt, verified GPS fix
- [ ] Phase 3 — FreeRTOS, NMEA parser, LCD display
- [ ] Phase 4 — LoRa transmission
- [ ] Phase 5 — Python CSV logger + Folium map

## Viewing Output
```bash
screen /dev/tty.usbserial-0001 115200
```