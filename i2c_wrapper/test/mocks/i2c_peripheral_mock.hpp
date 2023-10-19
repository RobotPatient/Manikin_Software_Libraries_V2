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

#ifndef I2C_PERIPHERAL_MOCK_HPP_
#define I2C_PERIPHERAL_MOCK_HPP_
#ifndef __arm__
#include <stdint.h>
#include <string.h>
#include "gmock/gmock.h"  // Brings in Google Mock.

class I2CPeripheralMock {
 public:
  MOCK_METHOD0(begin, void());
  MOCK_METHOD1(beginTransmission, void(uint8_t address));
  MOCK_METHOD1(write, size_t(uint8_t ucData));
  MOCK_METHOD2(write, size_t(const uint8_t *data, size_t quantity));
  MOCK_METHOD0(endTransmission, void());
  MOCK_METHOD2(requestFrom, uint8_t(uint8_t address, size_t quantity));
  MOCK_METHOD2(readBytes, size_t(uint8_t * buffer, size_t length));
  MOCK_METHOD0(read, int());
  MOCK_METHOD3(requestFrom, uint8_t(uint8_t address, size_t quantity, bool stopBit));
  MOCK_METHOD1(endTransmission, uint8_t(bool stopBit));
};
#endif  // __arm__
#endif  // I2C_PERIPHERAL_MOCK_HPP_
