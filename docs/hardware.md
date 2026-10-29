# Hardware setup and ESP-IDF integration

## Bench parts
- ESP32 DevKit using 3.3 V logic
- 3.3 V-compatible RS-485 transceiver (verify exact module)
- PC USB-to-RS-485 adapter
- Potentiometer to a supported ADC pin, or optional DS18B20 with its required pull-up

## Electrical cautions
- Do not apply 5 V logic/UART signals directly to ESP32 GPIO.
- Confirm the transceiver's logic supply and input thresholds.
- Wire A/B polarity according to the transceiver and adapter documentation.
- Terminate only the physical ends of the bus; avoid conflicting bias networks.
- Use protected low-voltage bench equipment, never mains or safety-critical machinery.

## Record your selected pins
Choose pins only after checking the exact ESP32 chip and module. Record the actual selections in this file.

| Signal | Role |
|---|---|
| UART TX | ESP32 TX to transceiver DI |
| UART RX | Transceiver RO to ESP32 RX |
| DE and /RE | Half-duplex direction control, as supported by the chosen transceiver |
| ADC | Potentiometer wiper in a safe input range |
| GPIO | Status LED |

## Integration sequence
1. Create an ESP-IDF app and select the exact target.
2. Configure UART baud, parity and stop bits (9600, 8E1 is a useful initial bench configuration).
3. Set RS-485 direction control according to the board and ESP-IDF UART API.
4. Buffer incoming UART bytes and identify complete RTU frames from inter-frame timing; one UART read is not necessarily one complete frame.
5. Call `modbus_process_request()` only for complete candidate frames, then transmit a nonzero response length.
6. Refresh input registers from the ADC/sensor task with safe synchronization.
7. Test using a PC Modbus master and logic analyzer; save real captures and results.

## Status
The portable RTU engine and host tests are provided. A generated ESP-IDF project, target pin map and physical RS-485/sensor measurements remain integration/validation tasks.
