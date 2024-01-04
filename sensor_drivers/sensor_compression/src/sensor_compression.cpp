/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
 *
 * The contents off this file are partially based off the sparkfun vl6180x library made by
 * Casey Kuhns and Kris Winer.
 * (https://github.com/sparkfun/SparkFun_ToF_Range_Finder-VL6180_Arduino_Library)
 *
 * The improvements made:
 * - Rewrite the library to use our own i2c module (wrapper library)
 * - Create unit-tests
 * - Made it pure cpp
 * - Simplified its interface to our needs
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
 * ToDo: implement 2.7 Range error codes
 * For documentation:
 * see https://www.st.com/resource/en/datasheet/vl6180x.pdf
 * also see https://www.st.com/resource/en/application_note/an4545-vl6180x-basic-ranging-application-note-stmicroelectronics.pdf
 *
 * Table 2.12 Scaling
 * For our purpose we set:
 * Scaling factor = 1 = proximity ranging (up to ~20 cm)
 * Note: The default scaling factor is 3 so we need to reset it to 1. (see SetVL6180xDefautSettings)
 *
***********************************************************************************************/

#include <sensor_compression.hpp>
#include <vl6180x_registers.hpp>

#ifdef __arm__
#ifdef Arduino
#include "Arduino.h"
#define sleep(ms) delay(ms)
#else
#define INCLUDE_vTaskDelay 1
#include <FreeRTOS.h>
#include <task.h>
#define sleep(ms) vTaskDelay(ms/portTICK_RATE_MS)
#endif
#elif _WIN32
#include<windows.h>
#define sleep(ms) Sleep(ms)
#else
#define sleep(ms) usleep(1000*ms)
#endif  // __arm__

void CompressionSensor::Initialize(I2CDriver* handle) {
  i2c_handle_ = handle;
  i2c_handle_->ChangeAddress(sensor_i2c_address_);
  InitVL6180X();
  SetVL6180xDefautSettings();
  sensor_data_.sample_num = 0;
}

SensorData CompressionSensor::GetSensorData() {
  uint8_t distance = GetDistance();
  sensor_data_.num_of_bytes = 1;
  sensor_data_.buffer[0] = distance;
  sensor_data_.sample_num++;
  sensor_data_.sensor_id = 0x01;
  sensor_data_.status = GetSensorRangeStatus();

  return sensor_data_;
}
/**
    * @brief Sets recommended settings required to be loaded onto the VL6180X during the
initialisation of the device
    *   See 4545 p.24/27 Section 9 SR03 settings
    * @return void
    */

uint8_t CompressionSensor::InitVL6180X(void) {
  uint8_t data = 0;

  data = i2c_handle_->ReadReg(kVl6180XSystemFreshOutOfReset);

  if (data != 1)
    return kVl6180XFailureReset;

  i2c_handle_->WriteReg(0x0207, 0x01);
  i2c_handle_->WriteReg(0x0208, 0x01);
  i2c_handle_->WriteReg(0x0096, 0x00);
  i2c_handle_->WriteReg(0x0097, 0xfd);
  i2c_handle_->WriteReg(0x00e3, 0x01);
  i2c_handle_->WriteReg(0x00e4, 0x03);
  i2c_handle_->WriteReg(0x00e5, 0x02);
  i2c_handle_->WriteReg(0x00e6, 0x01);
  i2c_handle_->WriteReg(0x00e7, 0x03);
  i2c_handle_->WriteReg(0x00f5, 0x02);
  i2c_handle_->WriteReg(0x00d9, 0x05);
  i2c_handle_->WriteReg(0x00db, 0xce);
  i2c_handle_->WriteReg(0x00dc, 0x03);
  i2c_handle_->WriteReg(0x00dd, 0xf8);
  i2c_handle_->WriteReg(0x009f, 0x00);
  i2c_handle_->WriteReg(0x00a3, 0x3c);
  i2c_handle_->WriteReg(0x00b7, 0x00);
  i2c_handle_->WriteReg(0x00bb, 0x3c);
  i2c_handle_->WriteReg(0x00b2, 0x09);
  i2c_handle_->WriteReg(0x00ca, 0x09);
  i2c_handle_->WriteReg(0x0198, 0x01);
  i2c_handle_->WriteReg(0x01b0, 0x17);
  i2c_handle_->WriteReg(0x01ad, 0x00);
  i2c_handle_->WriteReg(0x00ff, 0x05);
  i2c_handle_->WriteReg(0x0100, 0x05);
  i2c_handle_->WriteReg(0x0199, 0x05);
  i2c_handle_->WriteReg(0x01a6, 0x1b);
  i2c_handle_->WriteReg(0x01ac, 0x3e);
  i2c_handle_->WriteReg(0x01a7, 0x1f);
  i2c_handle_->WriteReg(0x0030, 0x00);
  return 0;
}

void CompressionSensor::SetVL6180xDefautSettings(void) {
  // Recommended settings from datasheet

  // Set GPIO1 high when sample complete
  i2c_handle_->WriteReg(kVl6180XSystemInterruptConfigGpio,
                        (4 << 3) | (4));

  // Set GPIO1 high when sample complete
  i2c_handle_->WriteReg(kVl6180XSystemModeGpio1, 0x10);

  // Set Avg sample period
  i2c_handle_->WriteReg(kVl6180XReadoutAveragingSamplePeriod, 0x30);
  // Set the ALS gain
  i2c_handle_->WriteReg(kVl6180XSysalsAnalogueGain, 0x46);
  // Set auto calibration period
  i2c_handle_->WriteReg(kVl6180XSysrangeVhvRepeatRate, 0xFF);
  // Set ALS integration time to 100ms
  i2c_handle_->WriteReg(kVl6180XSysalsIntegrationPeriod, 0x63);
  // perform a single temperature calibration
  i2c_handle_->WriteReg(kVl6180XSysrangeVhvRecalibrate, 0x01);

  // Optional settings from datasheet:
  // Set default ranging inter-measurement period to 100ms
  i2c_handle_->WriteReg(kVl6180XSysrangeIntermeasurementPeriod, 0x09);
  // Set default ALS inter-measurement period to 100ms
  i2c_handle_->WriteReg(kVl6180XSysalsIntermeasurementPeriod, 0x0A);
  // Configures interrupt on ‘New Sample Ready threshold event’
  i2c_handle_->WriteReg(kVl6180XSystemInterruptConfigGpio, 0x24);

  // Additional settings defaults from community
  i2c_handle_->WriteReg(kVl6180XSysrangeMaxConvergenceTime, 0x32);
  i2c_handle_->WriteReg(kVl6180XSysrangeRangeCheckEnables, 0x10 | 0x01);
  i2c_handle_->WriteReg16(kVl6180XSysrangeEarlyConvergenceEstimate, 0x7B);
  i2c_handle_->WriteReg16(kVl6180XSysalsIntegrationPeriod, 0x64);
  i2c_handle_->WriteReg(kVl6180XReadoutAveragingSamplePeriod, 0x30);
  i2c_handle_->WriteReg(kVl6180XSysalsAnalogueGain, 0x40);
  i2c_handle_->WriteReg(kVl6180XFirmwareResultScaler, 0x01);
}

/**
    * @brief Single shot distance mode see Application Note AN4545 p. 6/27
    *
    * @return Availability status
    */
uint8_t CompressionSensor::GetDistance(void) {
  uint8_t distance = 0;
  uint8_t interrupt_status = 0;
  uint8_t timeout_counter = 0;

  i2c_handle_->WriteReg(kVl6180XSysrangeStart, 0x01);
  interrupt_status = i2c_handle_->ReadReg(kVl6180XSysNewSampleReady);
  while (interrupt_status != kVl6180XSysNewSampleReadyStatusOK) {
    interrupt_status = i2c_handle_->ReadReg(kVl6180XSysNewSampleReady);
    timeout_counter++;
    if (timeout_counter > kMAX_SENSOR_READ_ATTEMPTS) { // Not likely but to avoid hangs...
      return 0; // ToDo: add timeout error flag.
    }
  }
  distance = i2c_handle_->ReadReg(kVl6180XResultRangeVal);
  i2c_handle_->WriteReg(kVl6180XSystemInterruptClear, 0x07);
  UpdateRangeError();
  return distance;
}

float CompressionSensor::GetAmbientLight(VL6180xAlsGain vl6180x_als_gain) {
  i2c_handle_->WriteReg(kVl6180XSysalsAnalogueGain, (0x40 | vl6180x_als_gain));

  // Start ALS Measurement
  i2c_handle_->WriteReg(kVl6180XSysalsStart, 0x01);

  sleep(kSAMPLE_TIME);  // give it time... (FreeRTOS compatible)

  i2c_handle_->WriteReg(kVl6180XSystemInterruptClear, 0x07);

  // Retrieve the Raw ALS value from the sensor
  unsigned int als_raw = i2c_handle_->ReadReg16(kVl6180XResultAlsVal);

  // Get Integration Period for calculation
  unsigned int als_integration_period_raw = i2c_handle_->ReadReg16(kVl6180XSysalsIntegrationPeriod);
  float als_integration_period = 100.0f / als_integration_period_raw;

  // Calculate actual LUX from Appnotes
  float als_gain = 0.0;

  switch (vl6180x_als_gain) {
    case kGain_20: als_gain = 20.0f;
      break;
    case kGain_10: als_gain = 10.32f;
      break;
    case kGain_5: als_gain = 5.21f;
      break;
    case kGain_2_5: als_gain = 2.60f;
      break;
    case kGain_1_67: als_gain = 1.72f;
      break;
    case kGain_1_25: als_gain = 1.28f;
      break;
    case kGain_1: als_gain = 1.01f;
      break;
    case kGain_40: als_gain = 40.0f;
      break;
  }

  // Calculate LUX from formula in AppNotes
  float als_calculated =
      0.32f * (static_cast<float>(als_raw) / als_gain) * als_integration_period;
  return als_calculated;
}

void CompressionSensor::GetIdentification(struct VL6180xIdentification *dest) {
  dest->id_model = i2c_handle_->ReadReg(kVl6180XIdentificationModelId);
  dest->id_model_rev_major =
      i2c_handle_->ReadReg(kVl6180XIdentificationModelRevMajor);
  dest->id_model_rev_minor =
      i2c_handle_->ReadReg(kVl6180XIdentificationModelRevMinor);
  dest->id_module_rev_major =
      i2c_handle_->ReadReg(kVl6180XIdentificationModuleRevMajor);
  dest->id_module_rev_minor =
      i2c_handle_->ReadReg(kVl6180XIdentificationModuleRevMinor);
  dest->id_date = i2c_handle_->ReadReg16(kVl6180XIdentificationDate);
  dest->id_time = i2c_handle_->ReadReg16(kVl6180XIdentificationTime);
}

uint8_t CompressionSensor::ChangeAddress(uint8_t old_address,
                                         uint8_t new_address) {
  // NOTICE:  IT APPEARS THAT CHANGING THE ADDRESS IS NOT STORED IN
  //          NON-VOLATILE MEMORY
  //  POWER CYCLING THE DEVICE REVERTS ADDRESS BACK TO 0x29
  if (old_address == new_address)
    return old_address;
  if (new_address > 127)
    return old_address;

  i2c_handle_->WriteReg(kVl6180Xi2CSlaveDeviceAddress, new_address);
  sensor_i2c_address_ = new_address;
  return i2c_handle_->ReadReg(kVl6180Xi2CSlaveDeviceAddress);
}

/**
    * @brief Get the availability status of the sensor
    *
    * @return Availability status
    */
const bool CompressionSensor::Available() {
  return i2c_handle_->SensorAvailable();
}

/**
    * @brief Updates the range_status property by reading the Register RESULT__RANGE_STATUS {0x4d}
    *
    * @return void
    */
void CompressionSensor::UpdateRangeError() {
  uint8_t _range_result = i2c_handle_->ReadReg(kVl6180XSysResultRangeStatus) >> 4;
  range_status = SENSOR_STATUS(_range_result);
}

/**
    * @brief Get the Sensor Range status of the sensor
    *
    * @return Sensor Range Status
    */
SENSOR_STATUS CompressionSensor::GetSensorRangeStatus() {
  return range_status;
}

void CompressionSensor::Uninitialize() {}

