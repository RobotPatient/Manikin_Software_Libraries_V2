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
#include <sensor_fingerposition.hpp>
#include <ads7138_registers.hpp>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::_;

constexpr uint16_t ProcessedVal(uint8_t *buffer) {
  return (buffer[0] << 4) | (buffer[1] >> 4);
}

uint8_t arb_test_buffer[16] = {0x05, 0x00, 0x85, 0x99,
                               0x91, 0x74, 0x55, 0x14};

uint16_t arb_test_buffer_processed[8] =
    {ProcessedVal(arb_test_buffer), ProcessedVal(arb_test_buffer + 2),
     ProcessedVal(arb_test_buffer + 4), ProcessedVal(arb_test_buffer + 6),
     ProcessedVal(arb_test_buffer + 8), ProcessedVal(arb_test_buffer + 10),
     ProcessedVal(arb_test_buffer + 12), ProcessedVal(arb_test_buffer + 14)};

uint16_t arb_test_buffer_reindexed[8] =
    {arb_test_buffer_processed[kLower], arb_test_buffer_processed[kMidL], arb_test_buffer_processed[kMidM],
     arb_test_buffer_processed[kMidH], arb_test_buffer_processed[kReL], arb_test_buffer_processed[kReH],
     arb_test_buffer_processed[kLiL], arb_test_buffer_processed[kLiH]};

int buffer_index = 0;

uint16_t AssembleRegister(uint8_t opcode, uint8_t regAddr) {
  uint16_t output = regAddr | (opcode << 8);
  return output;
}

void CopyArbTestBufferToBuffer(uint8_t *buffer, uint8_t num_of_bytes) {
  memcpy(buffer, arb_test_buffer + buffer_index, num_of_bytes);
  buffer_index += 2;
}

TEST(FingerPositionTest, initCalls) {
  /* Generated Parameters*/
  const uint16_t kReg1 = AssembleRegister(kSetBit, kPinConfig);
  const uint8_t kData1 = 0x00;
  const uint16_t kReg2 = AssembleRegister(kSetBit, kGeneralConfig);
  const uint8_t kData2 = 0x02;
  const uint16_t kReg3 = AssembleRegister(kSetBit, kAutoSeqSelChannel);
  const uint8_t kData3 = 0xFF;
  const uint16_t kReg4 = AssembleRegister(kSetBit, kSequenceConfig);
  const uint8_t kData4 = 0x01;
  /* Initialize handles and classes */
  I2CDriver i2c_mock_handle;
  FingerPositionSensor finger_pos_sensor;
  /* Setup mock calls */
  EXPECT_CALL(i2c_mock_handle, ChangeAddress(kAds7138Addr));
  {
    InSequence Seq;
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg1, kData1));
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg2, kData2));
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg3, kData3));
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg4, kData4));
  }
  /* Run the "real" call */
  finger_pos_sensor.Initialize(&i2c_mock_handle);
  Mock::VerifyAndClearExpectations(&i2c_mock_handle);
}

TEST(FingerPositionTest, GetSensorData) {
  /* Initialize handles and classes */
  const uint16_t kReg1 = AssembleRegister(kSetBit, kSequenceConfig);
  const uint8_t kData1 = 1 << 4;
  const uint16_t kReg2 = AssembleRegister(kClearBit, kSequenceConfig);
  const uint8_t kData2 = 1 << 4;
  // Initialize mocks
  I2CDriver i2c_mock_handle;
  FingerPositionSensor finger_pos_sensor;
  finger_pos_sensor.Initialize(&i2c_mock_handle);

  /* Setup mock calls */
  {
    InSequence seq;
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg1, kData1));
    EXPECT_CALL(i2c_mock_handle, ReadBytes(_, kReadNumOfBytes)).Times(kNumOfAdcChannels)
        .WillRepeatedly(Invoke(CopyArbTestBufferToBuffer));
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg2, kData2));
  }

  /* Run the "real" call */
  SensorData data = finger_pos_sensor.GetSensorData();

  /* Check the returned data with the preprocessed data */
  EXPECT_EQ(kNumOfSensorDataBytes, data.num_of_bytes);
  for (uint8_t i = 0; i < kNumOfAdcChannels; i++) {
    EXPECT_EQ(arb_test_buffer_reindexed[i], data.buffer[i]);
  }
  Mock::VerifyAndClearExpectations(&i2c_mock_handle);
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
