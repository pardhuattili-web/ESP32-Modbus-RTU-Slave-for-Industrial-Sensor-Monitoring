# Test plan

## Host test
Run `make test` and `python -m pytest -q`. Existing tests cover a read request and byte order, an illegal address exception, an unsupported function, a bad CRC, and a wrong slave address.

## Hardware validation (not yet claimed as run)
- [ ] ESP-IDF builds for the selected board target.
- [ ] PC master reads input and holding registers.
- [ ] Writable registers accept valid values; invalid values are rejected.
- [ ] CRC-invalid frames produce no response.
- [ ] Other slave addresses produce no response.
- [ ] Direction control switches only after UART transmission completes.
- [ ] ADC values update using documented scaling.
- [ ] Partial/long frames and UART errors recover cleanly.
- [ ] Logic analyzer capture confirms frame bytes and RTU timing.
- [ ] Test logs include firmware, toolchain, board, adapter, and wiring details.

Record actual PASS/FAIL/NOT RUN results; do not invent hardware measurements.
