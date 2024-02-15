
add_library(fram_driver fram_driver/src/MB85RS2MTA.c)
target_include_directories(fram_driver PUBLIC fram_driver/inc/)
target_link_libraries(fram_driver Universal_hal)

add_library(i2c_wrapper i2c_wrapper/src/i2c_helper_universal_hal.cpp)
target_include_directories(i2c_wrapper PUBLIC i2c_wrapper/src/)
target_link_libraries(i2c_wrapper Universal_hal)

add_library(sensor_compression sensor_drivers/sensor_compression/src/sensor_compression.cpp)
target_include_directories(sensor_compression PUBLIC sensor_drivers/sensor_base/src/ sensor_drivers/sensor_compression/src/)
target_link_libraries(sensor_compression i2c_wrapper FreeRTOS)

add_library(sensor_ventilation sensor_drivers/sensor_ventilation/src/sensor_ventilation.cpp)
target_include_directories(sensor_ventilation PUBLIC sensor_drivers/sensor_base/src/ sensor_drivers/sensor_ventilation/src/)
target_link_libraries(sensor_ventilation i2c_wrapper FreeRTOS)

add_library(sensor_fingerposition sensor_drivers/sensor_fingerposition/src/sensor_fingerposition.cpp)
target_include_directories(sensor_fingerposition PUBLIC sensor_drivers/sensor_base/src/ sensor_drivers/sensor_fingerposition/src/)
target_link_libraries(sensor_fingerposition i2c_wrapper FreeRTOS)

# for now unused: sensor_drivers/sensor_positioning/src/BMM150/bmm150.c
add_library(sensor_positioning sensor_drivers/sensor_positioning/src/sensor_positioning.cpp sensor_drivers/sensor_positioning/src/BMI270/bmi270.c sensor_drivers/sensor_positioning/src/BMI270/bmi2.c
        sensor_drivers/sensor_positioning/src/sensor_positioning_helper.h)
target_include_directories(sensor_positioning PUBLIC sensor_drivers/sensor_base/src/ sensor_drivers/sensor_positioning/src/ sensor_drivers/sensor_positioning/src/BMI270/)
target_link_libraries(sensor_positioning i2c_wrapper FreeRTOS)
