//
// Created by J.A. Korten on 14/02/2024.
//

#ifndef _SENSOR_POSITIONING_HELPER_H
#define _SENSOR_POSITIONING_HELPER_H

/*!
 *  @brief This function converts lsb to degree per second for 16 bit gyro at
 *  range 125, 250, 500, 1000 or 2000dps.
 *
 *  @param[in] val       : LSB from each axis.
 *  @param[in] dps       : Degree per second.
 *  @param[in] bit_width : Resolution for gyro.
 *
 *  @return Degree per second.
 */
static float lsb_to_dps(int16_t val, float dps, uint8_t bit_width)
{
  float half_scale = ((float)(1 << bit_width) / 2.0f);

  return (dps / ((half_scale))) * (val);
}





#endif //_SENSOR_POSITIONING_HELPER_H
