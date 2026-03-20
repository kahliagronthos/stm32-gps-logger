# STM32 GPS Logger

GPS data logger built on STM32F429I-DISC1 with NEO-6M GPS module.

## Hardware
- STM32F429I-DISC1 (Cortex-M4 @ 180MHz)
- NEO-6M GPS module
- CP2102 USB-UART adapter

## Wiring
- NEO-6M TX   →  STM32 PD2 (UART5 RX)
- STM32 PA9 (USART1 TX)  →  CP2102 RXD


## Viewing Output
```bash
screen /dev/tty.usbserial-0001 115200
```
