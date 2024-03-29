/**
 * @file S25FL064L.c
 * @author Hoog-v (you@domain.com)
 * @brief Source file for the S25FL064L driver
 * @version 0.1
 * @date 2024-03-29
 * 
 * @copyright Copyright (c) 2024 RobotPatients
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 */
#include "S25FL064L.h"
#include <hal_spi_host.h>

#define OPCODE_RDID 0x9F
#define OPCODE_WRITE 0x02
#define OPCODE_WREN 0x06
#define OPCODE_READ 0x03
#define OPCODE_SECTOR_ERASE  0x20 

#define FLASH_WRITABLE_AREA_OFFSET 0x400

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
  addr += FLASH_WRITABLE_AREA_OFFSET;

  prebuf[i++] = OPCODE_WRITE;
  prebuf[i++] = (uint8_t)(addr >> 16);
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
  addr += FLASH_WRITABLE_AREA_OFFSET;

  prebuf[i++] = OPCODE_WRITE;
  prebuf[i++] = (uint8_t)(addr >> 16);
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
  addr += FLASH_WRITABLE_AREA_OFFSET;

  buffer[i++] = OPCODE_READ;
  buffer[i++] = (uint8_t)(addr >> 16);
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
  addr += FLASH_WRITABLE_AREA_OFFSET;

  buffer[i++] = OPCODE_READ;
  buffer[i++] = (uint8_t)(addr >> 16);
  buffer[i++] = (uint8_t)(addr >> 8);
  buffer[i++] = (uint8_t)(addr & 0xFF);
  spi_host_start_transaction(fram_device.spi_bus, fram_device.cs_pin, SPI_EXTRA_OPT_USE_DEFAULT);
  spi_host_write_blocking(fram_device.spi_bus, buffer, i);
  spi_host_read_blocking(fram_device.spi_bus, read_buffer, amount_of_bytes);
  spi_host_end_transaction(fram_device.spi_bus, fram_device.cs_pin);
}

void fram_deinit(fram_dev_t fram_device) {

}