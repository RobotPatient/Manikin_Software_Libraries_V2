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
 *
***********************************************************************************************/

#ifndef SENSOR_POSITIONING_BMI270_REGISTERS_HPP_
#define SENSOR_POSITIONING_BMI270_REGISTERS_HPP_
#include <stdint.h>

const uint8_t kBMI270_COMMUNICATION_ERROR = -1;
const uint16_t kINIT_TIME = 450;

///*! @name BMI270 chip identifier */
//#define BMI270_CHIP_ID                         UINT8_C(0x24)
//
///*! @name BMI270 feature input start addresses */
//#define BMI270_CONFIG_ID_STRT_ADDR             UINT8_C(0x00)
//#define BMI270_MAX_BURST_LEN_STRT_ADDR         UINT8_C(0x02)
//#define BMI270_CRT_GYRO_SELF_TEST_STRT_ADDR    UINT8_C(0x03)
//#define BMI270_ABORT_STRT_ADDR                 UINT8_C(0x03)
//#define BMI270_AXIS_MAP_STRT_ADDR              UINT8_C(0x04)
//#define BMI270_GYRO_SELF_OFF_STRT_ADDR         UINT8_C(0x05)
//#define BMI270_NVM_PROG_PREP_STRT_ADDR         UINT8_C(0x05)
//#define BMI270_ANY_MOT_STRT_ADDR               UINT8_C(0x06)
//#define BMI270_NO_MOT_STRT_ADDR                UINT8_C(0x0A)
//#define BMI270_ORIENT_STRT_ADDR                UINT8_C(0x00)
//#define BMI270_HIGH_G_STRT_ADDR                UINT8_C(0x04)
//#define BMI270_LOW_G_STRT_ADDR                 UINT8_C(0x0A)
//#define BMI270_FLAT_STRT_ADDR                  UINT8_C(0x00)
//#define BMI270_SIG_MOT_STRT_ADDR               UINT8_C(0x04)
//#define BMI270_STEP_COUNT_STRT_ADDR            UINT8_C(0x00)
//#define BMI270_GYRO_USERGAIN_UPDATE_STRT_ADDR  UINT8_C(0x04)
//#define BMI270_TAP_DETECT_1_STRT_ADDR          UINT8_C(0x00)
//#define BMI270_TAP_DETECT_2_STRT_ADDR          UINT8_C(0x00)
//
///*! @name BMI270 feature output start addresses */
//#define BMI270_STEP_CNT_OUT_STRT_ADDR          UINT8_C(0x00)
//#define BMI270_STEP_ACT_OUT_STRT_ADDR          UINT8_C(0x04)
//#define BMI270_ORIENT_HIGH_G_OUT_STRT_ADDR     UINT8_C(0x06)
//#define BMI270_GYR_USER_GAIN_OUT_STRT_ADDR     UINT8_C(0x08)
//#define BMI270_GYRO_CROSS_SENSE_STRT_ADDR      UINT8_C(0x0C)
//#define BMI270_NVM_VFRM_OUT_STRT_ADDR          UINT8_C(0x0E)
//
///*! @name Defines maximum number of pages */
//#define BMI270_MAX_PAGE_NUM                    UINT8_C(8)
//
///*! @name Defines maximum number of feature input configurations */
//#define BMI270_MAX_FEAT_IN                     UINT8_C(23)
//
///*! @name Defines maximum number of feature outputs */
//#define BMI270_MAX_FEAT_OUT                    UINT8_C(8)
//
///*! @name Mask definitions for feature interrupt status bits */
//#define BMI270_SIG_MOT_STATUS_MASK             UINT8_C(0x01)
//#define BMI270_STEP_CNT_STATUS_MASK            UINT8_C(0x02)
//#define BMI270_HIGH_G_STATUS_MASK              UINT8_C(0x04)
//#define BMI270_LOW_G_STATUS_MASK               UINT8_C(0x04)
//#define BMI270_TAP_STATUS_MASK                 UINT8_C(0x08)
//#define BMI270_FLAT_STATUS_MASK                UINT8_C(0x10)
//#define BMI270_NO_MOT_STATUS_MASK              UINT8_C(0x20)
//#define BMI270_ANY_MOT_STATUS_MASK             UINT8_C(0x40)
//#define BMI270_ORIENT_STATUS_MASK              UINT8_C(0x80)
//#define BMI270_SINGLE_TAP_MASK                 UINT8_C(0x20)
//#define BMI270_DOUBLE_TAP_MASK                 UINT8_C(0x40)
//#define BMI270_TRIPLE_TAP_MASK                 UINT8_C(0x80)
//
///*! @name Status register for tap */
//#define BMI270_TAP_STATUS_REG                  UINT8_C(0x20)
//
///*! @name Macros for high-g interrupt axis/sign */
//#define BMI270_HIGH_G_DETECT_X                 UINT8_C(0x08)
//#define BMI270_HIGH_G_DETECT_Y                 UINT8_C(0x10)
//#define BMI270_HIGH_G_DETECT_Z                 UINT8_C(0x20)
//#define BMI270_HIGH_G_DETECT_SIGN              UINT8_C(0x40)
//
///*! @name Mask definitions for feature interrupt mapping bits */
//#define BMI270_INT_SIG_MOT_MASK                UINT8_C(0x01)
//#define BMI270_INT_STEP_COUNTER_MASK           UINT8_C(0x02)
//#define BMI270_INT_STEP_DETECTOR_MASK          UINT8_C(0x02)
//#define BMI270_INT_STEP_ACTIVITY_MASK          UINT8_C(0x02)
//#define BMI270_INT_HIGH_G_MASK                 UINT8_C(0x04)
//#define BMI270_INT_LOW_G_MASK                  UINT8_C(0x04)
//#define BMI270_INT_TAP_MASK                    UINT8_C(0x08)
//#define BMI270_INT_FLAT_MASK                   UINT8_C(0x10)
//#define BMI270_INT_NO_MOT_MASK                 UINT8_C(0x20)
//#define BMI270_INT_ANY_MOT_MASK                UINT8_C(0x40)
//#define BMI270_INT_ORIENT_MASK                 UINT8_C(0x80)
//#define BMI270_INT_SINGLE_TAP_MASK             UINT8_C(0x20)
//#define BMI270_INT_DOUBLE_TAP_MASK             UINT8_C(0x40)
//#define BMI270_INT_TRIPLE_TAP_MASK             UINT8_C(0x80)
//
///*! @name Defines maximum number of feature interrupts */
//#define BMI270_MAX_INT_MAP                     UINT8_C(14)


#endif  // SENSOR_POSITIONING_BMI270_REGISTERS_HPP_
