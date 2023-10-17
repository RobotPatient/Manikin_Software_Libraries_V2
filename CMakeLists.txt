add_library(fram_driver fram_driver/src/MB85RS2MTA.c)
target_include_directories(fram_driver PUBLIC fram_driver/inc/)
target_link_libraries(fram_driver Universal_hall)
