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

#include <gmock/gmock.h>
#include <i2c_helper.hpp>
#include <sensor_compression.hpp>
#include <vl6180x_registers.hpp>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Mock;

void InitVL6180xCalls(I2CDriver *i2c_handle_mock) {
  EXPECT_CALL(*i2c_handle_mock, ReadReg(kVl6180XSystemFreshOutOfReset))
      .WillOnce(Return(0x01));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x0207, 0x01));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x0208, 0x01));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x0096, 0x00));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x0097, 0xfd));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00e3, 0x00));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00e4, 0x04));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00e5, 0x02));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00e6, 0x01));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00e7, 0x03));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00f5, 0x02));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00d9, 0x05));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00db, 0xce));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00dc, 0x03));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00dd, 0xf8));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x009f, 0x00));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00a3, 0x3c));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00b7, 0x00));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00bb, 0x3c));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00b2, 0x09));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00ca, 0x09));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x0198, 0x01));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x01b0, 0x17));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x01ad, 0x00));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x00ff, 0x05));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x0100, 0x05));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x0199, 0x05));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x01a6, 0x1b));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x01ac, 0x3e));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x01a7, 0x1f));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(0x0030, 0x00));
}

void SetVL6180xDefaultSettingsCalls(I2CDriver *i2c_handle_mock) {
  // Set GPIO1 high when sample complete
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSystemInterruptConfigGpio, (4 << 3) | (4)));
  // Set GPIO1 high when sample complete
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSystemModeGpio1, 0x10));
  // Set Avg sample period
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XReadoutAveragingSamplePeriod, 0x30));
  // Set the ALS gain
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSysalsAnalogueGain, 0x46));
  // Set auto calibration period (Max = 255)/(OFF = 0)
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSysrangeVhvRepeatRate, 0xFF));
  // Set ALS integration time to 100ms
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSysalsIntegrationPeriod, 0x63));
  // perform a single temperature calibration
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSysrangeVhvRecalibrate, 0x01));

  // Optional settings from datasheet:
  // Set default ranging inter-measurement period to 100ms
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSysrangeIntermeasurementPeriod, 0x09));
  // Set default ALS inter-measurement period to 100ms
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSysalsIntermeasurementPeriod, 0x0A));
  // Configures interrupt on ‘New Sample Ready threshold event’
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSystemInterruptConfigGpio, 0x24));
  // Additional settings defaults from community
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSysrangeMaxConvergenceTime, 0x32));
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XSysrangeRangeCheckEnables, 0x10 | 0x01));
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg16(kVl6180XSysrangeEarlyConvergenceEstimate, 0x7B));
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg16(kVl6180XSysalsIntegrationPeriod, 0x64));
  EXPECT_CALL(*i2c_handle_mock,
              WriteReg(kVl6180XReadoutAveragingSamplePeriod, 0x30));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(kVl6180XSysalsAnalogueGain, 0x40));
  EXPECT_CALL(*i2c_handle_mock, WriteReg(kVl6180XFirmwareResultScaler, 0x01));
}

TEST(compressionTest, initCalls) {
  I2CDriver i2c_handle_mock;
  CompressionSensor CompSensor;
  EXPECT_CALL(i2c_handle_mock, ChangeAddress(kSensorAddr));
  {
    InSequence seq;
    InitVL6180xCalls(&i2c_handle_mock);
    SetVL6180xDefaultSettingsCalls(&i2c_handle_mock);
  }
  CompSensor.Initialize(&i2c_handle_mock);
  Mock::VerifyAndClearExpectations(&i2c_handle_mock);
}

TEST(compressionTest, GetSensorData) {
  I2CDriver i2c_handle_mock;
  SensorData ExpectedOutput;
  ExpectedOutput.buffer[0] = 0xAF;
  ExpectedOutput.num_of_bytes = 1;
  CompressionSensor CompSensor;
  CompSensor.Initialize(&i2c_handle_mock);
  EXPECT_CALL(i2c_handle_mock, WriteReg(kVl6180XSysrangeStart, 0x01));
  EXPECT_CALL(i2c_handle_mock, WriteReg(kVl6180XSystemInterruptClear, 0x07));
  EXPECT_CALL(i2c_handle_mock, ReadReg(kVl6180XResultRangeVal))
      .WillOnce(Return(ExpectedOutput.buffer[0]));
  // Do the "Real" call
  SensorData data = CompSensor.GetSensorData();
  EXPECT_EQ(ExpectedOutput.num_of_bytes, data.num_of_bytes);
  EXPECT_EQ(ExpectedOutput.buffer[0], data.buffer[0]);
  Mock::VerifyAndClearExpectations(&i2c_handle_mock);
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
