set(This i2c_wrapper_mock_test)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/i2c_helper.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/i2c_helper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks/i2c_peripheral_mock.hpp
        i2c_wrapper_mock_test.cc
        )

# We need this directory, and users of our library will need it too


add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/mocks/)
add_test(
        NAME ${This}
        COMMAND ${This}
)
