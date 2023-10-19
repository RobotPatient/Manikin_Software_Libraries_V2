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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <i2c_helper.hpp>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;

uint8_t kTestingBytes[8] = {0x10, 0x40, 0x30, 0x20, 0x10, 0xFF, 0x50, 0x01};

TEST(I2CWrapperTest, initCallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  I2CPeripheralMock i2c_peripheral_mock;
  I2CDriver driver = I2CDriver(&i2c_peripheral_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  EXPECT_CALL(i2c_peripheral_mock, begin());
  driver.Init();
}

TEST(I2CWrapperTest, write_regCallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CPeripheralMock i2c_peripheral_mock;
  I2CDriver driver = I2CDriver(&i2c_peripheral_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Parameters used in this test*/
  const uint16_t kReg = 0x0530;
  const uint16_t kData = 0x30;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  /* The expected function calls*/
  EXPECT_CALL(i2c_peripheral_mock, beginTransmission(kI2CAddress));
  {
    InSequence seq;
    EXPECT_CALL(i2c_peripheral_mock, write(kRegUpperByte));
    EXPECT_CALL(i2c_peripheral_mock, write(kRegLowerByte));
    EXPECT_CALL(i2c_peripheral_mock, write(kData));
  }
  EXPECT_CALL(i2c_peripheral_mock, endTransmission());
  /* The object method which calls to mock methods under the hood*/
  driver.WriteReg(kReg, kData);
}

TEST(I2CWrapperTest, write_reg16CallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CPeripheralMock i2c_peripheral_mock;
  I2CDriver driver = I2CDriver(&i2c_peripheral_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Parameters used in this test*/
  const uint16_t kReg = 0x0510;
  const uint16_t kData = 0x3050;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  const uint8_t kDataUpperByte = (kData >> 8) & 0xFF;
  const uint8_t kDataLowerByte = kData & 0xFF;
  /* The expected function calls*/
  EXPECT_CALL(i2c_peripheral_mock, beginTransmission(kI2CAddress));
  {
    InSequence seq;
    EXPECT_CALL(i2c_peripheral_mock, write(kRegUpperByte));
    EXPECT_CALL(i2c_peripheral_mock, write(kRegLowerByte));
    EXPECT_CALL(i2c_peripheral_mock, write(kDataUpperByte));
    EXPECT_CALL(i2c_peripheral_mock, write(kDataLowerByte));
  }
  EXPECT_CALL(i2c_peripheral_mock, endTransmission());
  /* The object method which calls to mock methods under the hood*/
  driver.WriteReg16(kReg, kData);
}

TEST(I2CWrapperTest, read_regCallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CPeripheralMock i2c_peripheral_mock;
  I2CDriver driver = I2CDriver(&i2c_peripheral_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Parameters used in this test*/
  const uint16_t kReg = 0x05;
  const uint8_t kDataToReturn = 0x53;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  const uint8_t kRequestAmountOfBytes = 1;
  /* The expected function calls*/
  EXPECT_CALL(i2c_peripheral_mock, beginTransmission(kI2CAddress));
  {
    InSequence seq;
    EXPECT_CALL(i2c_peripheral_mock, write(kRegUpperByte));
    EXPECT_CALL(i2c_peripheral_mock, write(kRegLowerByte));
  }
  EXPECT_CALL(i2c_peripheral_mock, endTransmission(false));
  EXPECT_CALL(i2c_peripheral_mock,
              requestFrom(kI2CAddress, kRequestAmountOfBytes));
  EXPECT_CALL(i2c_peripheral_mock, read())
      .WillRepeatedly(Return(kDataToReturn));
  /* The object method which calls to mock methods under the hood*/
  uint8_t data_returned = driver.ReadReg(kReg);
  /* Check if returned value matched the value that mock function returned*/
  EXPECT_EQ(data_returned, kDataToReturn);
}

TEST(I2CWrapperTest, read_reg16CallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CPeripheralMock i2c_peripheral_mock;
  I2CDriver driver = I2CDriver(&i2c_peripheral_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Parameters used in this test*/
  const uint16_t kReg = 0x0520;
  const uint16_t kDataToReturn = 0x5320;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  const uint8_t kDataUpperByte = (kDataToReturn >> 8) & 0xFF;
  const uint8_t kDataLowerByte = (kDataToReturn & 0xFF);
  const uint8_t kRequestAmountOfBytes = 2;  // We request 16-bits of data
  /* The expected function calls*/
  EXPECT_CALL(i2c_peripheral_mock, beginTransmission(kI2CAddress));
  {
    InSequence seq;
    EXPECT_CALL(i2c_peripheral_mock, write(kRegUpperByte));
    EXPECT_CALL(i2c_peripheral_mock, write(kRegLowerByte));
    EXPECT_CALL(i2c_peripheral_mock, endTransmission(false));
    EXPECT_CALL(i2c_peripheral_mock,
                requestFrom(kI2CAddress, kRequestAmountOfBytes));
    EXPECT_CALL(i2c_peripheral_mock, read())
        .WillOnce(Return(kDataUpperByte));
    EXPECT_CALL(i2c_peripheral_mock, read())
        .WillOnce(Return(kDataLowerByte));
  }
  /* The object method which calls to mock methods under the hood*/
  uint16_t data_returned = driver.ReadReg16(kReg);
  /* Check if returned value matched the value that mock function returned*/
  EXPECT_EQ(data_returned, kDataToReturn);
}

size_t CopyTestArray(uint8_t *buffer, size_t length) {
  memcpy(buffer, kTestingBytes, length);
  return length;
}

TEST(I2CWrapperTest, readBytesCallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CPeripheralMock i2c_peripheral_mock;
  I2CDriver driver = I2CDriver(&i2c_peripheral_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Generate mock method input parameters*/
  const uint8_t kRequestAmountOfBytes = 8;
  const bool kRequestStopBit = true;

  uint8_t test_buffer[8];
  /* The expected function calls*/
  EXPECT_CALL(i2c_peripheral_mock, requestFrom(kI2CAddress, kRequestAmountOfBytes, kRequestStopBit));
  EXPECT_CALL(i2c_peripheral_mock, readBytes(test_buffer, kRequestAmountOfBytes))
      .WillOnce(Invoke(CopyTestArray));
  /* The object method which calls to mock methods under the hood*/
  driver.ReadBytes(test_buffer, kRequestAmountOfBytes);
  /* Check if returned value matched the value that mock function returned*/
  for (uint8_t i = 0; i < kRequestAmountOfBytes; i++)
    EXPECT_EQ(test_buffer[i], kTestingBytes[i]);
}

TEST(I2CWrapperTest, sendBytesCallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CPeripheralMock i2c_peripheral_mock;
  I2CDriver driver = I2CDriver(&i2c_peripheral_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Parameters used in this test*/
  const uint8_t kRequestAmountOfBytes = 8;
  /* Generate mock method input parameters*/
  const bool kRequestStopBit = true;

  /* The expected function calls*/
  EXPECT_CALL(i2c_peripheral_mock, beginTransmission(kI2CAddress));
  EXPECT_CALL(i2c_peripheral_mock, write(kTestingBytes, kRequestAmountOfBytes));
  EXPECT_CALL(i2c_peripheral_mock, endTransmission(kRequestStopBit));
  /* The object method which calls to mock methods under the hood*/
  driver.SendBytes(kTestingBytes, kRequestAmountOfBytes);
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
