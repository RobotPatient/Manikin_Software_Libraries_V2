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
 * 
***********************************************************************************************/

#ifndef SENSOR_COMPRESSION_VL6180X_REGISTERS_HPP_
#define SENSOR_COMPRESSION_VL6180X_REGISTERS_HPP_
#include <stdint.h>

const uint8_t kVl6180XFailureReset = -1;
const uint16_t kVl6180XIdentificationModelId = 0x0000;
const uint16_t kVl6180XIdentificationModelRevMajor = 0x0001;
const uint16_t kVl6180XIdentificationModelRevMinor = 0x0002;
const uint16_t kVl6180XIdentificationModuleRevMajor = 0x0003;
const uint16_t kVl6180XIdentificationModuleRevMinor = 0x0004;
const uint16_t kVl6180XIdentificationDate = 0x0006;  // 16bit value
const uint16_t kVl6180XIdentificationTime = 0x0008;  // 16bit value

const uint16_t kVl6180XSystemModeGpio1 = 0x0011;
const uint16_t kVl6180XSystemInterruptConfigGpio = 0x0014;
const uint16_t kVl6180XSystemInterruptClear = 0x0015;
const uint16_t kVl6180XSystemFreshOutOfReset = 0x0016;

const uint16_t kVl6180XSysrangeStart = 0x0018;
const uint16_t kVl6180XSysrangeIntermeasurementPeriod = 0x001B;
const uint16_t kVl6180XSysrangeMaxConvergenceTime = 0x001C;
const uint16_t kVl6180XSysrangeEarlyConvergenceEstimate = 0x0022;
const uint16_t kVl6180XSysrangeRangeCheckEnables = 0x002D;
const uint16_t kVl6180XSysrangeVhvRecalibrate = 0x002E;
const uint16_t kVl6180XSysrangeVhvRepeatRate = 0x0031;

const uint16_t kVl6180XSysalsStart = 0x0038;
const uint16_t kVl6180XSysalsIntermeasurementPeriod = 0x003E;
const uint16_t kVl6180XSysalsAnalogueGain = 0x003F;
const uint16_t kVl6180XSysalsIntegrationPeriod = 0x0040;

const uint16_t kVl6180XSysNewSampleReady = 0x004F;
const uint16_t kVl6180XSysNewSampleReadyStatusOK = 0x04; // Poll RESULT__INTERRUPT_STATUS_GPIO {0x4f} register till bit 2 is set to 1.
const uint16_t kVl6180XSysResultRangeStatus = 0x004D;

const uint16_t kVl6180XResultAlsVal = 0x0050;
const uint16_t kVl6180XResultRangeVal = 0x0062;

const uint16_t kVl6180XReadoutAveragingSamplePeriod = 0x010A;
const uint16_t kVl6180XFirmwareResultScaler = 0x0120;
const uint16_t kVl6180Xi2CSlaveDeviceAddress = 0x0212;

const uint8_t kMAX_SENSOR_READ_ATTEMPTS = 150;
const uint8_t kSAMPLE_TIME = 100;

#endif  // SENSOR_COMPRESSION_VL6180X_REGISTERS_HPP_
