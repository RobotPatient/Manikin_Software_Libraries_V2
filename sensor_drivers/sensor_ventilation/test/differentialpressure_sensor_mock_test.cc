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
#include <sensor_differentialpressure.hpp>
#include <sdp810_registers.hpp>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::_;

uint8_t initialize_test_temp_buffer[2];
constexpr uint8_t arb_test_buffer[kSdp810BufferSize] = {0x20, 0x50, 0x70, 0x90,
                                              0x72, 0x10, 0x05, 0x02, 0x09};

void CopyExampleBufferToBuffer(uint8_t *buffer, uint8_t num_of_bytes) {
  memcpy(buffer, arb_test_buffer, num_of_bytes);
}

void CopyBufferToTestBuffer(uint8_t *buffer, uint8_t num_of_bytes) {
  memcpy(initialize_test_temp_buffer, buffer, num_of_bytes);
}

TEST(DifferentialPressureSensorTest, Initialize) {
  I2CDriver i2c_handle_mock;
  EXPECT_CALL(i2c_handle_mock, ChangeAddress(kSdp810I2CAddr));
  DifferentialPressureSensor DiffPressSensor;
  EXPECT_CALL(i2c_handle_mock, SendBytes(_, kSdp810InitCmdSize)).WillOnce(Invoke(CopyBufferToTestBuffer));
  DiffPressSensor.Initialize(&i2c_handle_mock);
  EXPECT_EQ(initialize_test_temp_buffer[0], kContMassFlowAvgMsb);
  EXPECT_EQ(initialize_test_temp_buffer[1], kContMassFlowAvgLsb);
  Mock::VerifyAndClearExpectations(&i2c_handle_mock);
}

TEST(DifferentialPressureSensorTest, GetSensorData) {
  // ToDo: Check the conversion-factor for the SDP810 sensor
  /* Generated Parameters */
  const int kConversionFactor = arb_test_buffer[6] << (kSdp810BufferSize - 1) | arb_test_buffer[7];
  const int kSensorOutput = (arb_test_buffer[0] << (kSdp810BufferSize - 1) | arb_test_buffer[1]) / kConversionFactor;
  I2CDriver i2c_handle_mock;
  DifferentialPressureSensor DiffPressSensor;
  DiffPressSensor.Initialize(&i2c_handle_mock);
  EXPECT_CALL(i2c_handle_mock, ReadBytes(_, kSdp810BufferSize)).WillOnce(Invoke(CopyExampleBufferToBuffer));
  SensorData data = DiffPressSensor.GetSensorData();
  EXPECT_EQ(data.buffer[0], kSensorOutput);
  EXPECT_EQ(data.num_of_bytes, kSdp810BytesToReturn);
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
