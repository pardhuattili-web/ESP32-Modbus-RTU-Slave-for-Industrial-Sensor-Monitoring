# Modbus register map

Addresses below are zero-based offsets on the wire. Use function 0x03 for holding registers and 0x04 for input registers. 16-bit register fields are sent most-significant byte first.

## Holding registers (0x03)

| Offset | Name | Access | Encoding |
|---:|---|---|---|
| 0 | Operating mode | Read/write | 0 automatic, 1 manual |
| 1 | Alarm threshold | Read/write | Application-defined scaling |
| 2–15 | Reserved | Read-only in reference model | Reserved |

The reference model marks only offsets 0 and 1 writable. Change the writable mask explicitly when extending the map.

## Input registers (0x04)

| Offset | Name | Access | Encoding |
|---:|---|---|---|
| 0 | ADC raw sample | Read-only | Nominal 0–4095 for a 12-bit measurement; check selected ADC configuration |
| 1 | Temperature | Read-only | Signed °C × 100, two's-complement 16-bit |
| 2 | Communication status | Read-only | Application-defined bit field |
| 3 | Accepted request count low word | Read-only | Counter snapshot |
| 4 | Rejected frame count low word | Read-only | Counter snapshot |
| 5–15 | Reserved | Read-only | Reserved |

Sensor service integration must refresh input registers. Add freshness flags or a documented sentinel rather than presenting stale readings as current.

## Functions and errors

Supported: 0x03, 0x04, 0x06, 0x10. Exceptions: 0x01 illegal function, 0x02 illegal address, 0x03 illegal value. Bad-CRC frames and requests for another slave address are ignored. Address 0 broadcast traffic is ignored by the current unicast reference API.

## Wire encoding

CRC-16/MODBUS uses initial value 0xFFFF and reflected polynomial 0xA001; transmit CRC low byte first. Normal 16-bit fields use big-endian order.
