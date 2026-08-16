
add_library(customDrivers INTERFACE)

target_include_directories(customDrivers
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..
)
