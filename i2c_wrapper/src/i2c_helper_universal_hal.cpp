/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
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
***********************************************************************************************/

#include "hal_i2c_host.h"
#include "i2c_helper.hpp"

constexpr uint8_t GetUpperByte(uint16_t number) {
  return (number >> 8) & 0xff;
}

constexpr uint8_t GetLowerByte(uint16_t number) {
  return (number & 0xFF);
}

void I2CDriver::Init() {
  i2c_host_init(i2c_peripheral_, I2C_CLK_SOURCE_USE_DEFAULT, 48000000, speed_, I2C_EXTRA_OPT_NONE);
}

void I2CDriver::ChangeAddress(uint8_t new_i2c_address) {
  i2c_addr_ = new_i2c_address;
}

void I2CDriver::WriteReg(uint16_t reg, uint8_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);
  const uint8_t WriteBuffer[3] = {kRegUpperByte, kRegLowerByte, data};

  i2c_host_write_blocking(i2c_peripheral_, i2c_addr_, WriteBuffer, sizeof(WriteBuffer), I2C_STOP_BIT);
}

void I2CDriver::WriteReg16(uint16_t reg, uint16_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);
  const uint8_t WriteBuffer[4] = {kRegUpperByte, kRegLowerByte, GetUpperByte(data), GetLowerByte(data)};

  i2c_host_write_blocking(i2c_peripheral_, i2c_addr_, WriteBuffer, sizeof(WriteBuffer), I2C_STOP_BIT);
}

uint8_t I2CDriver::ReadReg(uint16_t reg) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);
  const uint8_t WriteBuffer[2] = {kRegUpperByte, kRegLowerByte};
  uint8_t data;

  i2c_host_write_blocking(i2c_peripheral_, i2c_addr_, WriteBuffer, sizeof(WriteBuffer), I2C_STOP_BIT);
  i2c_host_read_blocking(i2c_peripheral_, i2c_addr_, &data, sizeof(data));
  return data;
}

uint16_t I2CDriver::ReadReg16(uint16_t reg) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);
  const uint8_t WriteBuffer[2] = {kRegUpperByte, kRegLowerByte};
  uint8_t ReadBuffer[2];
  uint16_t data;
  i2c_host_write_blocking(i2c_peripheral_, i2c_addr_, WriteBuffer, sizeof(WriteBuffer), I2C_STOP_BIT);
  i2c_host_read_blocking(i2c_peripheral_, i2c_addr_, ReadBuffer, sizeof(ReadBuffer));
  data = (ReadBuffer[0] << 8) | ReadBuffer[1];
  return data;
}

void I2CDriver::ReadBytes(uint8_t *buffer, uint8_t num_of_bytes) {
  i2c_host_read_blocking(i2c_peripheral_, i2c_addr_, buffer, num_of_bytes);
}

void I2CDriver::SendBytes(uint8_t *buffer, uint8_t num_of_bytes) {
  i2c_host_write_blocking(i2c_peripheral_, i2c_addr_, buffer, num_of_bytes, I2C_STOP_BIT);
}

bool I2CDriver::SensorAvailable() {
  const uint8_t identification = 0x00;
  uhal_status_t status = i2c_host_write_blocking(i2c_peripheral_, i2c_addr_, &identification, 0, I2C_STOP_BIT);
  return (status == 0);
}