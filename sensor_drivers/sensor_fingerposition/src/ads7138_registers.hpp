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

#ifndef ADS7138_REGISTERS_HPP_
#define ADS7138_REGISTERS_HPP_

inline constexpr uint8_t kReadNumOfBytes = 2;
inline constexpr uint8_t kNumOfAdcChannels = 8;

// 12-bit adc, 16-bit variable is the smallest size that fits this 12-bits.
// 16-bit is 2-bytes therefore the channels times 2
inline constexpr const uint8_t kNumOfSensorDataBytes = 2*kNumOfAdcChannels;

enum ChipRegisters {
  kGeneralConfig = 0x01,
  kPinConfig = 0x5,
  kSequenceConfig = 0x10,
  kAutoSeqSelChannel = 0x12,
};

enum ChipOpcodes {
  kSingleRead = 0b00010000,
  kSingleWrite = 0b00001000,
  kSetBit = 0b00011000,
  kClearBit = 0b00100000,
  kContinuousRead = 0b00110000,
  kContinuousWrite = 0b00101000,
};



#endif  // ADS7138_REGISTERS_HPP_
