/**
 * @file S25FL064L.h
 * @author Hoog-V (hogeweyv@gmail.com)
 * @brief Basic FRAM driver for the S25FL064L FLASH chip
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
#ifndef S25FL064L_H
#define S25FL064L_H
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
#endif // S25FL064L_H
