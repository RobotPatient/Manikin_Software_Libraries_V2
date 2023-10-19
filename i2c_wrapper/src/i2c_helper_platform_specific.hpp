#ifndef I2C_HELPER_PLATFORM_SPECIFIC_HPP
#define I2C_HELPER_PLATFORM_SPECIFIC_HPP

#ifdef Arduino
#include <Wire.h>
#define I2C_PERIPHERAL_T TwoWire*
#else
#include <hal_i2c_slave.h>
#define I2C_PERIPHERAL_T i2c_periph_inst_t
#endif

#endif
