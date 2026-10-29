#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H
#include <stddef.h>
#include <stdint.h>
#define MODBUS_MAX_ADU 256U
#define MODBUS_MAX_REGISTERS 32U
typedef struct {
    uint16_t input[MODBUS_MAX_REGISTERS];
    uint16_t holding[MODBUS_MAX_REGISTERS];
    uint16_t holding_count;
    uint16_t input_count;
    uint32_t holding_writable_mask;
    uint8_t slave_address;
    uint32_t accepted_requests;
    uint32_t rejected_frames;
    uint32_t exceptions;
} modbus_device_t;
size_t modbus_process_request(modbus_device_t *device, const uint8_t *request,
                              size_t request_length, uint8_t *response,
                              size_t response_capacity);
void modbus_device_init(modbus_device_t *device, uint8_t slave_address);
#endif
