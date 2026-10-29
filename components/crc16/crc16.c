#include "crc16.h"
uint16_t modbus_crc16(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFFU;
    size_t i;
    unsigned bit;
    if (data == 0 && length != 0U) return 0U;
    for (i = 0U; i < length; ++i) {
        crc ^= (uint16_t)data[i];
        for (bit = 0U; bit < 8U; ++bit)
            crc = (crc & 1U) ? (uint16_t)((crc >> 1U) ^ 0xA001U)
                             : (uint16_t)(crc >> 1U);
    }
    return crc;
}
