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
#include <unistd.h>


#include "BMI270/bmi2.h"
#include "BMI270/bmi270.h"
#include "BMI270/common/common.h"
#include "BMM150/bmm150.h"


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
  // This code is based on:
  // - examples from Bosch, but they are not too good in commenting why they do what...
  // - the Sparkfun BMI270 Arduino library without introducing Arduino dependencies

  int8_t err = BMI2_OK;
  uint8_t data = 0x00;

  data = i2c_handle_->ReadReg(kBMI270Addr);
  if (data != BMI2_ADDR_OK) // && (data != BMI2_OK))
//  if (data != BMI2_OK)
    return kBMI270_COMMUNICATION_ERROR;

  bmiSensor.chip_id = BMI270_CHIP_ID; // Note: not the i2c address but 0x24 see bst-bmi270-ds000.pdf
  bmiSensor.read = bmi2_i2c_read;
  bmiSensor.write = bmi2_i2c_write;
  bmiSensor.delay_us = bmi2_delay_us;
  bmiSensor.intf = BMI2_I2C_INTF;
  bmiSensor.intf_ptr = &accel_gyro_dev_info;
  bmiSensor.read_write_len = 30; // Was 30, but 32 in SparkFun version. Limitation of the Wire library -> @ Victor, we could change that as we don't use Wire

  bmiSensor.config_file_ptr = NULL; // Use the default BMI270 config file
  accel_gyro_dev_info._i2c_handle_ = i2c_handle_;
  accel_gyro_dev_info.dev_addr = bmiSensor.chip_id;

  /*
   * For future use:
   *
  bmm1.chip_id = BMM150_DEFAULT_I2C_ADDRESS;
  bmm1.read = bmi2_i2c_read;
  bmm1.write = bmi2_i2c_write;
  bmm1.delay_us = bmi2_delay_us;
  bmm1.intf = BMM150_I2C_INTF;
  bmm1.intf_ptr = &mag_dev_info;

  mag_dev_info._wire = _wire;
  mag_dev_info.dev_addr = bmm1.chip_id;
  */

  // ToDo: aan CMake toevoegen... ?

  err = bmi270_init(&bmiSensor);



  if(err != BMI2_OK) {
    return err;
  }

  return BMI2_OK;


  int8_t bmi270InitResult = bmi270_init(&bmiSensor);
  // print_rslt(bmi270InitResult); // might be interesting for future use...

  int8_t bmi270ConfigResult = configure_sensor(&bmiSensor);
  // print_rslt(bmi270ConfigResult); // might be interesting for future use...

  /*

  int8_t bmm150InitResult = bmm150_init(&bmm1);
  print_rslt(bmm150InitResult);

  int8_t bmm150ConfigResult = configure_sensor(&bmm1);
  print_rslt(bmm150ConfigResult);
  */

  bool success = bmi270InitResult == BMI2_OK && bmi270ConfigResult == BMI2_OK;
  // &&
  //               bmm150InitResult == BMM150_OK && bmm150ConfigResult == BMM150_OK;
  _initialized = success;

  return success;

  //return 0;
}

void PositioningSensor::Initialize(I2CDriver* handle) {
  handle->ChangeAddress(kSensorI2CAddress_);
  if (handle->SensorAvailable()) {
    i2c_handle_ = handle;
    i2c_handle_->ChangeAddress(kSensorI2CAddress_);
    int8_t status = InitBMI_Sensor();
    sensor_data_.sample_num = 0;
  }
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
  if (_initialized) {
    return i2c_handle_->SensorAvailable();
  } else {
    return false;
  }
}



int8_t PositioningSensor::bmi2_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
  if ((reg_data == NULL) || (len == 0) || (len > 32)) {
    return -1;
  }

  struct dev_info* dev_info = (struct dev_info*)intf_ptr;

  if (dev_info->_i2c_handle_->SendByte(reg_addr) == 0) {
    dev_info->_i2c_handle_->ReadBytes(reg_data, len);
  } else {
    return -1;
  }

  return 0;
}

Orientation3D PositioningSensor::GetGyroscopeInfo() {
  Orientation3D _result;
  struct bmi2_sens_data sensor_data = { { 0 } };
  int8_t rslt = bmi2_get_sensor_data(&sensor_data, &bmiSensor);

  if ((rslt == BMI2_OK) && (sensor_data.status & BMI2_DRDY_GYR)) {
    /* Converting lsb to degree per second for 16 bit gyro at 2000dps range. */
    //_result.x = lsb_to_dps(sensor_data.gyr.x, (float) GYRO_RANGE_2000_DPS, bmiSensor.resolution);
    //_result.y = lsb_to_dps(sensor_data.gyr.y, (float) GYRO_RANGE_2000_DPS, bmiSensor.resolution);
    //_result.z = lsb_to_dps(sensor_data.gyr.z, (float) GYRO_RANGE_2000_DPS, bmiSensor.resolution);
  }
  return _result;
}

Orientation3D PositioningSensor::GetAcceleroInfo() {
  Orientation3D _result;
  struct bmi2_sens_data sensor_data = { { 0 } };
  int8_t rslt = bmi2_get_sensor_data(&sensor_data, &bmiSensor);

  if ((rslt == BMI2_OK) && (sensor_data.status & BMI2_DRDY_ACC)) {

      /* Converting lsb to meter per second squared for 16 bit accelerometer at 2G range. */
      //_result.x = lsb_to_mps2(sensor_data.acc.x, (float)ACCEL_RANGE_2G, bmiSensor.resolution);
      //_result.y = lsb_to_mps2(sensor_data.acc.x, (float)ACCEL_RANGE_2G, bmiSensor.resolution);
      //_result.z = lsb_to_mps2(sensor_data.acc.x, (float)ACCEL_RANGE_2G, bmiSensor.resolution);
    }
  return _result;
}

Orientation3D PositioningSensor::GetMagnetoInfo() {
  Orientation3D _result;
  return _result;
}

/**
      * @brief Get the data from the three different sensors (Gyroscope, Accelerometer and Magnetometer)
      *
      * @return Availability status
      */

SensorData PositioningSensor::GetSensorData() {
  SensorData sensor_data_;

#ifndef USE_MAGNETOMETER
  sensor_data_.num_of_bytes = 6; // g.x/y/z; a.x/y/z
#endif

#ifdef USE_MAGNETOMETER
  sensor_data_.num_of_bytes = 9; // g.x/y/z; a.x/y/z; m.x/y/z;
  Orientation3D magnetOrientation = GetMagnetoInfo();
#endif
  Orientation3D gyroscOrientation = GetGyroscopeInfo();
  Orientation3D accelerOrientation = GetAcceleroInfo();

  sensor_data_.buffer[0] = gyroscOrientation.x;
  sensor_data_.buffer[1] = gyroscOrientation.y;
  sensor_data_.buffer[2] = gyroscOrientation.z;

  sensor_data_.buffer[3] = accelerOrientation.x;
  sensor_data_.buffer[4] = accelerOrientation.y;
  sensor_data_.buffer[5] = accelerOrientation.z;

#ifdef USE_MAGNETOMETER
  sensor_data_.buffer[6] = magnetOrientation.x;
    sensor_data_.buffer[7] = magnetOrientation.y;
    sensor_data_.buffer[8] = magnetOrientation.z;
#endif

  sensor_data_.sample_num++;
  sensor_data_.sensor_id = POSITIONING_SENSOR;
  //sensor_data_.status = GetSensorRangeStatus();

  return sensor_data_;
}

int8_t PositioningSensor::bmi2_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
  if ((reg_data == NULL) || (len == 0) || (len > 32)) {
    return -1;
  }

  struct dev_info* dev_info = (struct dev_info*)intf_ptr;
  //uint8_t dev_id = dev_info->dev_addr;
  dev_info->_i2c_handle_->SendByte(reg_addr);
  if (dev_info->_i2c_handle_->SendBytes(reg_data, len) != 0) {
    return -1;
  }

  return 0;
}

void PositioningSensor::bmi2_delay_us(uint32_t period, void *intf_ptr)
{
  uint32_t msec = period / 1000U;

  if (period % 1000U != 0U)
    msec++;

  vTaskDelay(msec); // see https://community.bosch-sensortec.com/t5/MEMS-sensors-forum/Question-BMI270-BMI2-E-CONFIG-LOAD/td-p/21113
}

int8_t PositioningSensor::configure_sensor(struct bmi2_dev *dev)
{
  int8_t rslt;
  /*
  uint8_t sens_list[2] = { BMI2_ACCEL, BMI2_GYRO };

  struct bmi2_int_pin_config int_pin_cfg;
  int_pin_cfg.pin_type = BMI2_INT1;
  int_pin_cfg.int_latch = BMI2_INT_NON_LATCH;
  int_pin_cfg.pin_cfg[0].lvl = BMI2_INT_ACTIVE_HIGH;
  int_pin_cfg.pin_cfg[0].od = BMI2_INT_PUSH_PULL;
  int_pin_cfg.pin_cfg[0].output_en = BMI2_INT_OUTPUT_ENABLE;
  int_pin_cfg.pin_cfg[0].input_en = BMI2_INT_INPUT_DISABLE;

  struct bmi2_sens_config sens_cfg[2];
  sens_cfg[0].type = BMI2_ACCEL;
  sens_cfg[0].cfg.acc.bwp = BMI2_ACC_OSR2_AVG2;
  sens_cfg[0].cfg.acc.odr = BMI2_ACC_ODR_100HZ;
  sens_cfg[0].cfg.acc.filter_perf = BMI2_PERF_OPT_MODE;
  sens_cfg[0].cfg.acc.range = BMI2_ACC_RANGE_4G;

  sens_cfg[1].type = BMI2_GYRO;
  sens_cfg[1].cfg.gyr.filter_perf = BMI2_PERF_OPT_MODE;
  sens_cfg[1].cfg.gyr.bwp = BMI2_GYR_OSR2_MODE;
  sens_cfg[1].cfg.gyr.odr = BMI2_GYR_ODR_100HZ;
  sens_cfg[1].cfg.gyr.range = BMI2_GYR_RANGE_2000;
  sens_cfg[1].cfg.gyr.ois_range = BMI2_GYR_OIS_2000;

  rslt = bmi2_set_int_pin_config(&int_pin_cfg, dev);
  if (rslt != BMI2_OK)
    return rslt;

  rslt = bmi2_map_data_int(BMI2_DRDY_INT, BMI2_INT1, dev);
  if (rslt != BMI2_OK)
    return rslt;

  rslt = bmi2_set_sensor_config(sens_cfg, 2, dev);
  if (rslt != BMI2_OK)
    return rslt;

  rslt = bmi2_sensor_enable(sens_list, 2, dev);
  if (rslt != BMI2_OK)
    return rslt;


   */
  return rslt;
}
