/* *******************************************************************************************
 * Copyright (c) 2024 by RobotPatient Simulators
 *
 * Authors: Victor Hogeweij and Johan Korten
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
 * For BMI270:
 * See https://www.bosch-sensortec.com/media/boschsensortec/downloads/application_notes_1/bst-bmi270-an002-01.pdf
***********************************************************************************************/

#include <stdio.h>
#include <math.h>

//#include "BMI270/bmi2.h"
//#include "BMI270/bmi270.h"
//#include "BMI270/common/common.h"

#include "sensor_positioning.hpp"
#include "BMI270/bmi270_registers.hpp"


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

// For now, we will only implement BMI270.
// For BMM150 we need a second i2c_handle etc.
// From a Software Engineering perspective it would be better to add abstractions.
// For now, we decided not to add abstractions and focus more for performance instead.
// JK, Jan 2024

void PositioningSensor::SensorTest() {
  //
}

/**
    * @brief Sets recommended settings required to be loaded onto the BMI270 during the
initialisation of the device
    *   See 1b. p.9/98
    * @return void
    */

void PositioningSensor::InitBMI270Registers() {



  //i2c_handle_->WriteReg(0x007C, 0x00);
  //sleep(kINIT_TIME);
  //i2c_handle_->WriteReg(0x0059, 0x00);

  // init_array
  //SensorTest();



  // burst write reg

  //i2c_handle_->WriteReg(0x0059, 0x01);

}

/**
    * @brief Sets recommended settings required to be loaded onto the BMIxxx (e.g. BMI270)
    *  during the  initialisation of the device.
    *  See 1a. p.8/98
    *
    *  Should be only called via Initialize(...)
    *
    * Note: We assume the i2c wire is already initialized correctly.
    * @return uint8_t
    */

uint8_t PositioningSensor::InitBMI_Sensor(void) {
  uint8_t data = 0xFF;

  data = i2c_handle_->ReadReg(kBMI270Addr);
  if (data != 0x01)
    return kBMI270_COMMUNICATION_ERROR;

  bmiSensor.chip_id = kBMI270Addr;
  bmiSensor.read = bmi2_i2c_read;
  bmiSensor.write = bmi2_i2c_write;
  bmiSensor.delay_us = bmi2_delay_us;
  bmiSensor.intf = BMI2_I2C_INTF;
  //bmiSensor.intf_ptr = &accel_gyro_dev_info;
  bmiSensor.read_write_len = 30; // Limitation of the Wire library
  bmiSensor.config_file_ptr = NULL; // Use the default BMI270 config file

  /*

  accel_gyro_dev_info._wire = _wire;
  accel_gyro_dev_info.dev_addr = bmi2.chip_id;

  bmm1.chip_id = BMM150_DEFAULT_I2C_ADDRESS;
  bmm1.read = bmi2_i2c_read;
  bmm1.write = bmi2_i2c_write;
  bmm1.delay_us = bmi2_delay_us;
  bmm1.intf = BMM150_I2C_INTF;
  bmm1.intf_ptr = &mag_dev_info;

  mag_dev_info._wire = _wire;
  mag_dev_info.dev_addr = bmm1.chip_id;

  int8_t bmi270InitResult = bmi270_init(&bmi2);
  print_rslt(bmi270InitResult);

  int8_t bmi270ConfigResult = configure_sensor(&bmi2);
  print_rslt(bmi270ConfigResult);

  int8_t bmm150InitResult = bmm150_init(&bmm1);
  print_rslt(bmm150InitResult);

  int8_t bmm150ConfigResult = configure_sensor(&bmm1);
  print_rslt(bmm150ConfigResult);

  bool success = bmi270InitResult == BMI2_OK && bmi270ConfigResult == BMI2_OK &&
                 bmm150InitResult == BMM150_OK && bmm150ConfigResult == BMM150_OK;
  _initialized = success;

  return success;
  */
  return 0;
}

SensorData PositioningSensor::GetSensorData() {
  /*
  uint8_t distance = GetDistance();
  sensor_data_.num_of_bytes = 1;
  sensor_data_.buffer[0] = distance;
  sensor_data_.sample_num++;
  sensor_data_.sensor_id = 0x01;
  sensor_data_.status = GetSensorRangeStatus();
  */
  return sensor_data_;
}

void PositioningSensor::Initialize(I2CDriver* handle) {
  i2c_handle_ = handle;
  i2c_handle_->ChangeAddress(kSensorI2CAddress_);
  InitBMI_Sensor();
  //SetVL6180xDefautSettings();
  sensor_data_.sample_num = 0;
}

/**
    * @brief Uninitialize the sensor(s)
    */
void PositioningSensor::Uninitialize() {

}

/**
      * @brief Get the availability status of the sensor
      *
      * @return Availability status
      */

const bool PositioningSensor::Available() {
  return i2c_handle_->SensorAvailable();
}



int8_t PositioningSensor::bmi2_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
  if ((reg_data == NULL) || (len == 0) || (len > 32)) {
    return -1;
  }
  uint8_t bytes_received;

  struct dev_info* dev_info = (struct dev_info*)intf_ptr;
  uint8_t dev_id = dev_info->dev_addr;

  //dev_info->_wire->beginTransmission(dev_id);
  dev_info->_i2c_handle_->beginTransmission(dev_id);
  dev_info->_wire->write(reg_addr);
  if (dev_info->_wire->endTransmission() == 0) {
    bytes_received = dev_info->_wire->requestFrom(dev_id, len);
    // Optionally, throw an error if bytes_received != len
    for (uint16_t i = 0; i < bytes_received; i++)
    {
      reg_data[i] = dev_info->_wire->read();
    }
  } else {
    return -1;
  }

  return 0;
}

int8_t PositioningSensor::bmi2_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
  if ((reg_data == NULL) || (len == 0) || (len > 32)) {
    return -1;
  }

  struct dev_info* dev_info = (struct dev_info*)intf_ptr;
  uint8_t dev_id = dev_info->dev_addr;
  dev_info->_wire->beginTransmission(dev_id);
  dev_info->_wire->write(reg_addr);
  for (uint16_t i = 0; i < len; i++)
  {
    dev_info->_wire->write(reg_data[i]);
  }
  if (dev_info->_wire->endTransmission() != 0) {
    return -1;
  }

  return 0;
}

void PositioningSensor::bmi2_delay_us(uint32_t period, void *intf_ptr)
{
  delayMicroseconds(period);
}