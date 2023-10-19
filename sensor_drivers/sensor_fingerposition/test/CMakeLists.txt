set(This fingerposition_sensor_mock_test)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../i2c/test/mocks/i2c_helper.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../i2c/test/mocks/i2c_peripheral_mock.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensor_fingerposition.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensor_fingerposition.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_base/src/sensor_base.hpp
        fingerposition_sensor_mock_test.cc
        )

# We need this directory, and users of our library will need it too

add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../i2c/test/mocks/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_base/src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../src/
                                          .)
add_test(
        NAME ${This}
        COMMAND ${This}
)
