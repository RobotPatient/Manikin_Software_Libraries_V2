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
***********************************************************************************************/
#ifndef SENSOR_COMPRESSION_HPP_
#define SENSOR_COMPRESSION_HPP_

#include <sensor_base.hpp>
#include <i2c_helper.hpp>

inline constexpr uint8_t kSensorAddr = 0x29;

// Data sheet shows gain values as binary list
enum VL6180xAlsGain {
  kGain_20 = 0,        // Actual ALS Gain of 20
  kGain_10,                  // Actual ALS Gain of 10.32
  kGain_5,                   // Actual ALS Gain of 5.21
  kGain_2_5,                 // Actual ALS Gain of 2.60
  kGain_1_67,                // Actual ALS Gain of 1.72
  kGain_1_25,                // Actual ALS Gain of 1.28
  kGain_1,                   // Actual ALS Gain of 1.01
  kGain_40,                  // Actual ALS Gain of 40
};

enum SensorStatus { // 3.1 Range error codes Application notes p. 8/27
    STATUS_OK = 0,
    SYSTEM_ERROR_1 = 1,
    SYSTEM_ERROR_2 = 2,
    SYSTEM_ERROR_3 = 3,
    SYSTEM_ERROR_4 = 4,
    SYSTEM_ERROR_5 = 5,
    ECE_CHECK_FAILED = 6,
    MAX_CONVERGENCE_EXCEEDED = 7,
    RANGE_IGNORE_TH_CHECK_FAILED = 8,
    SNR_ERROR = 11,
    RAW_RANGE_UNDERFLOW = 12,
    RAW_RANGE_OVERFLOW = 13,
    RANGING_UNDERFLOW = 14,
    RANGING_OVERFLOW = 15,
};

struct VL6180xIdentification {
  uint8_t id_model;
  uint8_t id_model_rev_major;
  uint8_t id_model_rev_minor;
  uint8_t id_module_rev_major;
  uint8_t id_module_rev_minor;
  uint16_t id_date;
  uint16_t id_time;
};




class CompressionSensor : public UniversalSensor {
 public:
  CompressionSensor() : UniversalSensor() {}

  /**
   * @brief Initialises the sensor with its default settings
   * 
   * @param handle Pointer to I2C slave driver instance
  */
  void Initialize(I2CDriver* handle) override;

  /**
  * @brief Get the sensortype
  * 
  * @return unique sensor identifier
  */
  const uint8_t GetSensorType() override {
    return SensorType_;
  }

  /**
   * @brief Read the sensor
   * 
   * @return SensorData_t SensorData struct with the sensordata, sample_num and sensor_id
   */
  SensorData GetSensorData() override;

  /**
    * @brief Get the availability status of the sensor
    *
    * @return Availability status
    */
  const bool Available() override;

  /**
    * @brief Get the Sensor Range status of the sensor
    *
    * @return Sensor Range Status
    */
  SensorStatus GetSensorRangeStatus();

  /**
  * @brief Uninitialize the sensor
  */
  void Uninitialize() override;
  ~CompressionSensor() {
    Uninitialize();
  }

 private:
  const uint8_t SensorType_ = 0x01;
  uint8_t sensor_i2c_address_ = kSensorAddr;
  SensorStatus range_status = SensorStatus::STATUS_OK;

  SensorData sensor_data_{};
  I2CDriver *i2c_handle_;

// Low level driver functions:
  uint8_t InitVL6180X(void);
  void SetVL6180xDefautSettings(void);
  uint8_t GetDistance(void);
  float GetAmbientLight(VL6180xAlsGain vl6180x_als_gain);
  void GetIdentification(struct VL6180xIdentification *dest);
  uint8_t ChangeAddress(uint8_t old_address, uint8_t new_address);

  /**
    * @brief Updates range_status property based on RESULT_RANGE_STATUS {0x4d} Register
    *
    * @return void
    */
  void UpdateRangeError();

};
#endif  // SENSOR_COMPRESSION_HPP_
