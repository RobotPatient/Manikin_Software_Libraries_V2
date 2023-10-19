//
// Created by victor on 17-10-23.
//

#ifndef ATMELSAMD21_MB85RS2MTA_H
#define ATMELSAMD21_MB85RS2MTA_H
#include <stdint.h>
#include <stdio.h>
#include <hal_spi_host.h>
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
typedef struct {
  spi_host_inst_t spi_bus;
  gpio_pin_t cs_pin;
}fram_dev_t;

void fram_init(fram_dev_t fram_device);

void fram_wren(fram_dev_t fram_device);

void fram_write_byte(fram_dev_t fram_device, uint32_t addr, uint8_t byte);

void fram_write_bytes(fram_dev_t fram_device, uint32_t addr, const uint8_t* bytes, size_t amount_of_bytes);

void fram_read_byte(fram_dev_t fram_device, uint32_t addr, uint8_t* read_buffer);

void fram_read_bytes(fram_dev_t fram_device, uint32_t addr, uint8_t* read_buffer, size_t amount_of_bytes);

void fram_deinit(fram_dev_t fram_device);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // ATMELSAMD21_MB85RS2MTA_H
