#include "MB85RS2MTA.h"
#include <hal_spi_host.h>

#define OPCODE_RDID 0b10011111
#define OPCODE_WRITE 0b0010
#define OPCODE_WREN 0b0110
#define OPCODE_READ 0b0011

void fram_wren(fram_dev_t fram_device) {
  uint8_t opcode = OPCODE_WREN;
  spi_host_start_transaction(fram_device.spi_bus, fram_device.cs_pin, SPI_EXTRA_OPT_USE_DEFAULT);
  spi_host_write_blocking(fram_device.spi_bus, &opcode, 1);
  spi_host_end_transaction(fram_device.spi_bus, fram_device.cs_pin);
}

void fram_init(fram_dev_t fram_device) {
  uint8_t buffer[4] = {OPCODE_RDID, 0, 0, 0};
  spi_host_start_transaction(fram_device.spi_bus, fram_device.cs_pin, SPI_EXTRA_OPT_USE_DEFAULT);
  spi_host_write_blocking(fram_device.spi_bus, buffer, 1);
  spi_host_read_blocking(fram_device.spi_bus, buffer, 4);
  spi_host_end_transaction(fram_device.spi_bus, fram_device.cs_pin);

}

void fram_write_byte(fram_dev_t fram_device, uint32_t addr, uint8_t byte){
  uint8_t prebuf[5];
  uint8_t i = 0;

  prebuf[i++] = OPCODE_WRITE;
  prebuf[i++] = (uint8_t)(addr >> 24);
  prebuf[i++] = (uint8_t)(addr >> 8);
  prebuf[i++] = (uint8_t)(addr & 0xFF);
  prebuf[i++] = (uint8_t) byte;
  spi_host_start_transaction(fram_device.spi_bus, fram_device.cs_pin, SPI_EXTRA_OPT_USE_DEFAULT);
  spi_host_write_blocking(fram_device.spi_bus, prebuf, i);
  spi_host_end_transaction(fram_device.spi_bus, fram_device.cs_pin);
}

void fram_write_bytes(fram_dev_t fram_device, uint32_t addr, const uint8_t* bytes, size_t amount_of_bytes){
  uint8_t prebuf[4];
  uint8_t i = 0;

  prebuf[i++] = OPCODE_WRITE;
  prebuf[i++] = (uint8_t)(addr >> 24);
  prebuf[i++] = (uint8_t)(addr >> 8);
  prebuf[i++] = (uint8_t)(addr & 0xFF);
  spi_host_start_transaction(fram_device.spi_bus, fram_device.cs_pin, SPI_EXTRA_OPT_USE_DEFAULT);
  spi_host_write_blocking(fram_device.spi_bus, prebuf, i);
  spi_host_write_blocking(fram_device.spi_bus, bytes, amount_of_bytes);
  spi_host_end_transaction(fram_device.spi_bus, fram_device.cs_pin);
}

void fram_read_byte(fram_dev_t fram_device, uint32_t addr, uint8_t* read_buffer){
  uint8_t buffer[4];
  uint8_t i = 0;
  buffer[i++] = OPCODE_READ;
  buffer[i++] = (uint8_t)(addr >> 24);
  buffer[i++] = (uint8_t)(addr >> 8);
  buffer[i++] = (uint8_t)(addr & 0xFF);
  spi_host_start_transaction(fram_device.spi_bus, fram_device.cs_pin, SPI_EXTRA_OPT_USE_DEFAULT);
  spi_host_write_blocking(fram_device.spi_bus, buffer, i);
  spi_host_read_blocking(fram_device.spi_bus, read_buffer, 1);
  spi_host_end_transaction(fram_device.spi_bus, fram_device.cs_pin);
}

void fram_read_bytes(fram_dev_t fram_device, uint32_t addr, uint8_t* read_buffer, size_t amount_of_bytes){
  uint8_t buffer[4];
  uint8_t i = 0;
  buffer[i++] = OPCODE_READ;
  buffer[i++] = (uint8_t)(addr >> 24);
  buffer[i++] = (uint8_t)(addr >> 8);
  buffer[i++] = (uint8_t)(addr & 0xFF);
  spi_host_start_transaction(fram_device.spi_bus, fram_device.cs_pin, SPI_EXTRA_OPT_USE_DEFAULT);
  spi_host_write_blocking(fram_device.spi_bus, buffer, i);
  spi_host_read_blocking(fram_device.spi_bus, read_buffer, amount_of_bytes);
  spi_host_end_transaction(fram_device.spi_bus, fram_device.cs_pin);
}

void fram_deinit(fram_dev_t fram_device) {

}