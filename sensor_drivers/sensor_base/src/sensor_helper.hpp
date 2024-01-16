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

#ifndef SENSOR_HELPER_HPP
#define SENSOR_HELPER_HPP

#include <stdio.h>
#include <math.h>

inline constexpr uint8_t kMaxAmountOfSensorBytes = 9;
/**
 * @brief Sensordata struct contains the read sensor data with samplenum and sensortype
 *
 * @note buffer contains the raw sensordata
 *
 */
typedef struct SensorData {
  uint16_t sample_num;                          /**< index / counter */
  uint16_t sensor_id;                           /**< high byte: real sensor_id as defined in SensorType enum, low byte: subsensor (e.g. 0..8 for finger position sensor) */
  uint16_t buffer[kMaxAmountOfSensorBytes];     /**< actual data of sensor */
  uint8_t num_of_bytes;                         /**< number of bytes as stored in the buffer */
  uint8_t status;                               /**< sensor status */
} SensorData_t;

enum BareSensorType {
    NO_SENSOR = 0xFF,
    UNKNOWN_SENSOR = 0xF0,
    COMPRESSION_SENSOR = 0x01,
    VENTILATION_SENSOR = 0x02,
    COMPRESSION_POSITION_SENSOR = 0x03,
    POSITIONING_SENSOR = 0x04,
    HEAD_POSITIONING_SENSOR = 0x05,
    BODY_POSITIONING_SENSOR = 0x06
};

/*!
 * @brief This function converts lsb to degree per second for 16 bit gyro at
 * range 125, 250, 500, 1000 or 2000dps.
 */
static float lsb_to_dps(int16_t val, float dps, uint8_t bit_width)
{
  double power = 2;

  float half_scale = (float)((pow((double)power, (double)bit_width) / 2.0f));

  return (dps / (half_scale)) * (val);
}


#endif  // SENSOR_HELPER_HPP_
