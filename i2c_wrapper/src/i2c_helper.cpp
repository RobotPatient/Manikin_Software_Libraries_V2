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
 *
 * Note to the developer:
 * This class is only to be used in case of Arduino
 *
***********************************************************************************************/

#include <i2c_helper.hpp>

constexpr uint8_t GetUpperByte(uint16_t number) {
  return (number >> 8) & 0xff;
}

constexpr uint8_t GetLowerByte(uint16_t number) {
  return (number & 0xFF);
}

void I2CDriver::Init() {
  i2c_peripheral_->begin();
}

void I2CDriver::ChangeAddress(uint8_t new_i2c_address) {
  i2c_addr_ = new_i2c_address;
}

void I2CDriver::WriteReg(uint16_t reg, uint8_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(i2c_addr_);
  i2c_peripheral_->write(kRegUpperByte);
  i2c_peripheral_->write(kRegLowerByte);
  i2c_peripheral_->write(data);
  i2c_peripheral_->endTransmission();
}

void I2CDriver::WriteReg16(uint16_t reg, uint16_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(i2c_addr_);
  i2c_peripheral_->write(kRegUpperByte);
  i2c_peripheral_->write(kRegLowerByte);

  uint8_t temp;
  temp = GetUpperByte(data);
  i2c_peripheral_->write(temp);
  temp = GetLowerByte(data);
  i2c_peripheral_->write(temp);
  i2c_peripheral_->endTransmission();
}

uint8_t I2CDriver::ReadReg(uint16_t reg) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(i2c_addr_);
  i2c_peripheral_->write(GetUpperByte(kRegUpperByte));
  i2c_peripheral_->write(GetLowerByte(kRegLowerByte));
  i2c_peripheral_->endTransmission(false);
  i2c_peripheral_->requestFrom(i2c_addr_, 1);
  uint8_t data = i2c_peripheral_->read();
  return data;
}

uint16_t I2CDriver::ReadReg16(uint16_t reg) {
  uint8_t data_low;
  uint8_t data_high;
  uint16_t data;

  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(i2c_addr_);
  i2c_peripheral_->write(kRegUpperByte);
  i2c_peripheral_->write(kRegLowerByte);
  i2c_peripheral_->endTransmission(false);
  i2c_peripheral_->requestFrom(i2c_addr_, 2);
  data_high = i2c_peripheral_->read();
  data_low = i2c_peripheral_->read();
  data = (data_high << 8) | data_low;
  return data;
}

void I2CDriver::ReadBytes(uint8_t *buffer, uint8_t num_of_bytes) {
  i2c_peripheral_->requestFrom(i2c_addr_, num_of_bytes, true);
  i2c_peripheral_->readBytes(buffer, num_of_bytes);
}

uint8_t I2CDriver::SendBytes(const uint8_t *buffer, uint8_t num_of_bytes) {
  i2c_peripheral_->beginTransmission(i2c_addr_);
  i2c_peripheral_->write(buffer, num_of_bytes);
  return i2c_peripheral_->endTransmission(true);
}

uint8_t I2CDriver::SendByte(const uint8_t data) {
  i2c_peripheral_->beginTransmission(i2c_addr_);
  i2c_peripheral_->write(data, 1);
  return i2c_peripheral_->endTransmission(true);
}