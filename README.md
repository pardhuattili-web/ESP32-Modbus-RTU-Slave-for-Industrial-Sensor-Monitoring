# ESP32 Modbus RTU Slave for Industrial Sensor Monitoring

A modular Modbus RTU sensor node for ESP32, designed to expose live measurements and configurable parameters over half-duplex RS-485.

> **Status:** Portable protocol implementation and host tests are provided as the first verified layer. ESP-IDF peripheral integration and physical RS-485 validation must be completed on the target board. Do not treat a passing host build as proof of a hardware-tested product.

## Features

- Modbus RTU CRC-16 validation and strict request-length checking.
- Slave-address filtering and exception responses.
- Function codes: **0x03 Read Holding Registers**, **0x04 Read Input Registers**, **0x06 Write Single Register**, **0x10 Write Multiple Registers**.
- Portable register-map module with validated writable configuration values.
- Sensor values exposed as scaled integers (no floating point on the wire).
- ESP-IDF UART/RS-485 integration boundary and board bring-up instructions.
- Host-side protocol tests and GitHub Actions CI.

## Architecture

```text
ADC / optional DS18B20
        |
        v
  Sensor service ------> Input register map (read-only)
                              |
Modbus master <-> RS-485 <-> UART RX/TX <-> RTU parser
                              |
                       Holding register map
                              |
                     validated configuration
                              |
                     diagnostics / counters
```

## Repository layout

```text
main/                 ESP-IDF application integration
components/modbus/    Portable Modbus RTU parser and register model
components/crc16/     CRC-16/MODBUS implementation
host/                 Host test harness and frame generator
tests/                Unit and regression tests
docs/                 Register map, wiring, testing and architecture
.github/workflows/    Automated host checks
```

## Quick start: host tests

Requirements: GCC or Clang, Make, Python 3.10+, pytest.

```bash
make test
python -m pip install -r requirements-dev.txt
python -m pytest -q
```

The host suite exercises protocol logic without ESP32 hardware and is suitable for development on Linux, macOS, or WSL with a C compiler and Python.

## ESP-IDF build

Install ESP-IDF using Espressif's official setup instructions, export the environment, and select the correct target for your board. This repository is structured for ESP-IDF, but check the current target integration status before flashing: the host-tested protocol layer is separate from the board-specific UART task and pin configuration.

Typical workflow once the ESP-IDF application integration is configured:

```bash
idf.py set-target esp32
idf.py menuconfig
idf.py build
idf.py flash monitor
```

## Hardware

- ESP32 DevKit board (3.3 V logic)
- 3.3 V-compatible RS-485 transceiver such as MAX3485 or an equivalent module
- USB-to-RS-485 adapter for PC testing
- Potentiometer connected to an ADC-capable pin, or an optional DS18B20 temperature sensor
- Appropriate pull-up and termination/bias components as required by bus topology

Read [hardware setup](docs/hardware.md) and your transceiver datasheet before wiring. Tie the ESP32 and transceiver logic grounds as required by the design. Do not connect 5 V UART signals directly to ESP32 GPIO. Use termination at the physical ends of the bus and avoid duplicate bias networks.

## Register map

See [docs/register-map.md](docs/register-map.md) for exact addresses, scaling, permissions, and supported function codes.

- Multi-byte protocol values use Modbus network byte order (most significant byte first).
- Modbus RTU CRC bytes are transmitted low byte first.
- Internal register addresses are zero-based; the document shows wire offsets explicitly.
- Sensor values are scaled signed/unsigned integers, not IEEE-754 floats.

## Test scenarios

The suite covers CRC known vectors, request framing, supported functions, illegal addresses/quantities, read-only writes, invalid configuration values, wrong slave addresses, and corrupted CRC frames. Hardware validation should additionally confirm UART timing, RS-485 direction control, bus wiring, and real sensor acquisition.

## Safety and scope

This is a learning/portfolio project, not a safety-certified industrial controller. Use isolated or appropriately protected low-voltage test equipment. Never connect the prototype directly to mains or safety-critical machinery. Document actual hardware results; do not claim physical tests unless they were run and recorded.

## License

MIT. See [LICENSE](LICENSE).
