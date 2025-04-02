include_directories(${CMAKE_SOURCE_DIR}/src)

# Find Google Test
find_package(GTest REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/tests)

file(GLOB_RECURSE MALASIM_TEST_SOURCES
  "*.cpp"
  "helpers/*.cpp"
  "Core/Random/*.cpp"
  "Core/Scheduler/*.cpp"
  "Configuration/*.cpp"
  "Environment/*.cpp"
  "GIS/SpatialDataTest/*.cpp"
  "GIS/AdminLevelManagerTest/*.cpp"
  "GIS/SpatialDataTest/*.cpp"
  "Population/*.cpp"
)


# Add test executable
add_executable(malasim_test
  ${MALASIM_TEST_SOURCES}
)

add_dependencies( malasim_test MalaSimCore)

# Link the core library and GTest to the test executable
target_link_libraries(malasim_test PRIVATE
  GTest::gtest_main GTest::gmock
  MalaSimCore
)

set_property(TARGET malasim_test PROPERTY CXX_STANDARD 20)

# Add tests
add_test(NAME malasim_test COMMAND ${CMAKE_BINARY_DIR}/bin/malasim_test)
