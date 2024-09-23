include_directories(${CMAKE_SOURCE_DIR}/src)

# Find Google Test
find_package(GTest REQUIRED)

# Add test executable
add_executable(runTests
    # Add your test files here
    example_test.cpp
)

# Link the core library and GTest to the test executable
target_link_libraries(runTests PRIVATE
  GTest::gtest_main
  MalaSimCore
)

# Add tests
add_test(NAME example_test COMMAND ${CMAKE_BINARY_DIR}/bin/runTests)
