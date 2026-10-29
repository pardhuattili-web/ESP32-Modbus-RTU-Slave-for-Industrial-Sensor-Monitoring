# ESP-IDF application integration

The portable RTU engine is in `components/modbus/`. Integrate it into an ESP-IDF `app_main()` by initializing UART, RS-485 direction GPIO, ADC/sensor service, and an RX task that buffers bytes until a complete RTU frame boundary is detected.

Example API:

```c
modbus_device_t device;
uint8_t response[MODBUS_MAX_ADU];
modbus_device_init(&device, 1U);
size_t response_length = modbus_process_request(
    &device, request, request_length, response, sizeof(response));
```

The ESP-IDF task, UART configuration, RTU framing timer, ADC driver and exact pin map must be compiled and hardware-tested on the chosen target. Do not transmit if the core API returns zero.
