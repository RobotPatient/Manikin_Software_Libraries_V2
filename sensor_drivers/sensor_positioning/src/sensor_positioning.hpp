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
***********************************************************************************************/

#ifndef SENSOR_POSITIONING_HPP_
#define SENSOR_POSITIONING_HPP_

#include <sensor_base.hpp>

inline constexpr uint8_t kBMI270Addr = 0x10;

class PositioningSensor : public UniversalSensor {
public:
    PositioningSensor() : UniversalSensor() {}

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
    * @brief Uninitialize the sensor
    */

    void Uninitialize() override;
    ~PositioningSensor() {
      Uninitialize();
    }

    /**
      * @brief Get the availability status of the sensor
      *
      * @return Availability status
      */

    const bool Available() override {
      return false;  // TODO
    }

private:
    const uint8_t SensorType_ = 0x03;
    const uint8_t kSensorI2CAddress_ = kBMI270Addr;
    I2CDriver *i2c_handle_;
    //SensorData sensor_data_{};

    //void initDefaultRead(void);
    //void readADC(uint16_t *dest);
    //uint16_t assembleRegister(uint8_t opcode, uint8_t reg_addr);

    // Low Level I2C communication:
    //void writeRegister(uint8_t reg_addr, uint8_t data);
    //void setRegister(uint8_t reg_addr, uint8_t data);
    //void clearRegister(uint8_t reg_addr, uint8_t data);
    //uint8_t getRegister(uint8_t register_addr);

    //void startReadSEQ(void);
    //void stopReadSEQ(void);
    //void reindexArray(uint16_t *dest, uint16_t *original);
    //void getReading(uint8_t *buf);
};

typedef PositioningSensor HeadPositioningSensor;
//typedef PositioningSensor BodyPositioningSensor;

#endif  // SENSOR_POSITIONING_HPP_