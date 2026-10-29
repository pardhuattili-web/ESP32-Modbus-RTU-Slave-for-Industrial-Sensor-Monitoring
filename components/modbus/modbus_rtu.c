#include "modbus_rtu.h"
#include "crc16.h"
#include <stddef.h>
#include <string.h>

enum { EX_ILLEGAL_FUNCTION = 1, EX_ILLEGAL_ADDRESS = 2, EX_ILLEGAL_VALUE = 3 };
static uint16_t get_u16(const uint8_t *p) { return (uint16_t)(((uint16_t)p[0] << 8U) | p[1]); }
static void put_u16(uint8_t *p, uint16_t v) { p[0] = (uint8_t)(v >> 8U); p[1] = (uint8_t)v; }
static size_t finish_response(uint8_t *out, size_t length, size_t cap) {
    uint16_t crc;
    if (cap < length + 2U) return 0U;
    crc = modbus_crc16(out, length);
    out[length] = (uint8_t)crc;
    out[length + 1U] = (uint8_t)(crc >> 8U);
    return length + 2U;
}
static size_t exception_response(uint8_t addr, uint8_t fn, uint8_t code, uint8_t *out, size_t cap) {
    if (cap < 5U) return 0U;
    out[0] = addr; out[1] = (uint8_t)(fn | 0x80U); out[2] = code;
    return finish_response(out, 3U, cap);
}
void modbus_device_init(modbus_device_t *d, uint8_t addr) {
    if (d == NULL) return;
    memset(d, 0, sizeof(*d));
    d->slave_address = addr; d->holding_count = 16U; d->input_count = 16U;
    d->holding_writable_mask = 0x00000003U;
}
size_t modbus_process_request(modbus_device_t *d, const uint8_t *req, size_t len,
                              uint8_t *out, size_t cap) {
    uint8_t addr, fn, code = 0U;
    uint16_t received_crc, calculated_crc, start, qty, value;
    const uint16_t *source;
    size_t i, response_len = 0U;
    if (d == NULL || req == NULL || out == NULL || len < 4U || len > MODBUS_MAX_ADU) return 0U;
    addr = req[0]; fn = req[1];
    if (addr == 0U || addr != d->slave_address) return 0U;
    received_crc = (uint16_t)(req[len - 2U] | ((uint16_t)req[len - 1U] << 8U));
    calculated_crc = modbus_crc16(req, len - 2U);
    if (received_crc != calculated_crc) { d->rejected_frames++; return 0U; }

    if (fn == 0x03U || fn == 0x04U) {
        if (len != 8U) code = EX_ILLEGAL_VALUE;
        else {
            start = get_u16(&req[2]); qty = get_u16(&req[4]);
            if (qty == 0U || qty > 125U) code = EX_ILLEGAL_VALUE;
            else {
                source = (fn == 0x03U) ? d->holding : d->input;
                value = (fn == 0x03U) ? d->holding_count : d->input_count;
                if (start >= value || qty > (uint16_t)(value - start)) code = EX_ILLEGAL_ADDRESS;
                else if (cap < (size_t)(5U + 2U * qty)) return 0U;
                else {
                    out[0] = addr; out[1] = fn; out[2] = (uint8_t)(2U * qty);
                    for (i = 0U; i < qty; ++i) put_u16(&out[3U + i * 2U], source[start + i]);
                    response_len = 3U + 2U * qty;
                }
            }
        }
    } else if (fn == 0x06U) {
        if (len != 8U) code = EX_ILLEGAL_VALUE;
        else {
            start = get_u16(&req[2]); value = get_u16(&req[4]);
            if (start >= d->holding_count || start >= 32U ||
                (d->holding_writable_mask & (1UL << start)) == 0U) code = EX_ILLEGAL_ADDRESS;
            else if (start == 0U && value > 1U) code = EX_ILLEGAL_VALUE;
            else { d->holding[start] = value; memcpy(out, req, 6U); response_len = 6U; }
        }
    } else if (fn == 0x10U) {
        if (len < 9U) code = EX_ILLEGAL_VALUE;
        else {
            start = get_u16(&req[2]); qty = get_u16(&req[4]);
            if (qty == 0U || qty > 123U || req[6] != (uint8_t)(qty * 2U) ||
                len != (size_t)(9U + req[6])) code = EX_ILLEGAL_VALUE;
            else if (start >= d->holding_count || qty > (uint16_t)(d->holding_count - start))
                code = EX_ILLEGAL_ADDRESS;
            else {
                /* Validate the entire request first so an error causes no partial writes. */
                for (i = 0U; i < qty; ++i) {
                    uint16_t index = (uint16_t)(start + i);
                    uint16_t candidate = get_u16(&req[7U + i * 2U]);
                    if (index >= 32U || (d->holding_writable_mask & (1UL << index)) == 0U) {
                        code = EX_ILLEGAL_ADDRESS; break;
                    }
                    if (index == 0U && candidate > 1U) { code = EX_ILLEGAL_VALUE; break; }
                }
                if (code == 0U) {
                    for (i = 0U; i < qty; ++i) d->holding[start + i] = get_u16(&req[7U + i * 2U]);
                    out[0] = addr; out[1] = fn; put_u16(&out[2], start); put_u16(&out[4], qty);
                    response_len = 6U;
                }
            }
        }
    } else code = EX_ILLEGAL_FUNCTION;

    if (code != 0U) {
        d->exceptions++;
        return exception_response(addr, fn, code, out, cap);
    }
    if (response_len != 0U) {
        response_len = finish_response(out, response_len, cap);
        if (response_len != 0U) d->accepted_requests++;
    }
    return response_len;
}
