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
#include "BMI270/bmi270.h"

inline constexpr uint8_t kBMI270Addr = 0x24;

// #define USE_MAGNETOMETER // uncomment if you want to use the MAGNETOMETER too.

struct dev_info {
    I2CDriver *_i2c_handle_;
    uint8_t dev_addr;
};

struct Orientation3D {
    float x, y, z;
};

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

    const bool Available() override;

    void SensorTest();

private:
    const uint8_t SensorType_ = 0x03;
    const uint8_t kSensorI2CAddress_ = kBMI270Addr;
    I2CDriver *i2c_handle_;
    SensorData sensor_data_{};

    struct dev_info accel_gyro_dev_info;
    // struct dev_info mag_dev_info; // for future use

    struct bmi2_dev bmiSensor;
    // struct bmm150_dev bmmSensor; // for future use

    uint8_t InitBMI_Sensor(void);
    void SetBMI270DefautSettings(void);
    void InitBMI270Registers();

    static int8_t bmi2_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
    static int8_t bmi2_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
    static void bmi2_delay_us(uint32_t period, void *intf_ptr);

    int8_t configure_sensor(struct bmi2_dev *dev);

    bool _initialized = false;

    Orientation3D GetGyroscopeInfo();
    Orientation3D GetAcceleroInfo();
    Orientation3D GetMagnetoInfo(); // (future extension BMM)

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