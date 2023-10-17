#include "MB85RS2MTA.h"
#include <hal_spi_host.h>

#define OPCODE_RDID 0b10011111

void fram_init(fram_dev_t fram_device) {
  uint8_t buffer[4] = {OPCODE_RDID, 0, 0, 0};
  spi_host_start_transaction(fram_device.spi_bus, fram_device.cs_pin, SPI_EXTRA_OPT_USE_DEFAULT);
  spi_host_write_blocking(fram_device.spi_bus, buffer, 1);
  spi_host_read_blocking(fram_device.spi_bus, buffer, 4);
  spi_host_end_transaction(fram_device.spi_bus, fram_device.cs_pin);

}

void fram_write_byte(fram_dev_t fram_device, uint32_t addr, uint8_t byte){

}

void fram_write_bytes(fram_dev_t fram_device, uint32_t addr, uint8_t* bytes, size_t amount_of_bytes){

}

void fram_read_byte(fram_dev_t fram_device, uint8_t* read_buffer){

}

void fram_read_bytes(fram_dev_t fram_device, uint8_t* read_buffer, size_t amount_of_bytes){

}

void fram_deinit(fram_dev_t fram_device) {

}