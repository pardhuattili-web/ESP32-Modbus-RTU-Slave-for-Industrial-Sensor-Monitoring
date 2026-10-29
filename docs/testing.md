# Test plan

## Host test
Run `make test` and `python -m pytest -q`. Current host tests cover a read request and byte order, illegal-address exception, unsupported-function exception, corrupted CRC rejection, and wrong-slave-address filtering.

## Hardware validation (not yet claimed as run)
- [ ] ESP-IDF builds for the selected board target.
- [ ] A PC Modbus master reads input and holding registers.
- [ ] Writable registers accept valid values and reject invalid values.
- [ ] CRC-invalid frames produce no response.
- [ ] Other slave addresses produce no response.
- [ ] RS-485 direction switches only after UART transmission completes.
- [ ] ADC values update using documented scaling.
- [ ] Partial frames and UART errors recover cleanly.
- [ ] Logic analyzer capture confirms frame bytes and RTU timing.
- [ ] Logs include firmware, toolchain, board, adapter, and wiring details.

Record actual PASS/FAIL/NOT RUN results; do not invent hardware measurements.
