include_directories(${CMAKE_SOURCE_DIR}/src)

# Find Google Test
find_package(GTest REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/tests)

file(GLOB_RECURSE MALASIM_TEST_SOURCES
  "*.cpp"
  "helpers/*.cpp"
  "Core/Random/*.cpp"
  "Configuration/*.cpp"
)


# Add test executable
add_executable(runTests
  ${MALASIM_TEST_SOURCES}
        Configuration/yaml_simulation_timeframe_conversion_test.cpp
        Configuration/yaml_spatial_settings_conversion_test.cpp
        Configuration/yaml_seasonality_settings_conversion_test.cpp
        Configuration/yaml_movement_settings_conversion_test.cpp
        Configuration/yaml_parasite_parameters_conversion_test.cpp
        Configuration/yaml_immune_system_parameters_conversion_test.cpp
        Configuration/yaml_epidemiological_parameters_conversion_test.cpp
)

add_dependencies(runTests MalaSimCore)

# Link the core library and GTest to the test executable
target_link_libraries(runTests PRIVATE
  GTest::gtest_main
  MalaSimCore
)

set_property(TARGET runTests PROPERTY CXX_STANDARD 20)

# Add tests
add_test(NAME malasim_test COMMAND ${CMAKE_BINARY_DIR}/bin/runTests)
