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

#ifndef I2C_HELPER_HPP_
#define I2C_HELPER_HPP_
#include <stdint.h>

#ifdef __arm__
#include "i2c_helper_platform_specific.hpp"
#else
#include <i2c_peripheral_mock.hpp>
#define I2C_PERIPHERAL_T I2CPeripheralMock*
#endif

typedef enum {
  kI2cSpeed_100KHz = 100000, kI2cSpeed_400KHz = 400000,
} I2CSpeed;


class I2CDriver {
 public:
  I2CDriver(I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed, uint8_t i2c_addr) {
    this->i2c_peripheral_ = i2c_peripheral;
    this->i2c_addr_ = i2c_addr;
    this->speed_ = speed;
  }

  I2CDriver(I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed) {
    this->i2c_peripheral_ = i2c_peripheral;
    this->speed_ = speed;
  }

  void Init();
  void WriteReg(uint16_t reg, uint8_t data);
  void WriteReg16(uint16_t reg, uint16_t data);
  uint8_t ReadReg(uint16_t reg);
  uint16_t ReadReg16(uint16_t reg);

  void ReadBytes(uint8_t *buffer, uint8_t num_of_bytes);
  uint8_t SendBytes(const uint8_t *buffer, uint8_t num_of_bytes);
  uint8_t SendByte(const uint8_t data);
  void ChangeAddress(uint8_t new_i2c_address);

  bool SensorAvailable();
 private:

  uint8_t i2c_addr_;
  I2C_PERIPHERAL_T i2c_peripheral_;
  I2CSpeed speed_;

};

#endif  // I2C_HELPER_HPP_
