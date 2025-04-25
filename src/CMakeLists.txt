find_package(fmt CONFIG REQUIRED)
find_package(GSL REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(spdlog REQUIRED)
find_package(date CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

option(ENABLE_TRAVEL_TRACKING "Enable tracking of individual travel data and generating travel reports" OFF)

if(ENABLE_TRAVEL_TRACKING)
  message(STATUS "Travel tracking feature is enabled")
  # Add definitions or configurations specific to the travel tracking feature
  # add_definitions(-DENABLE_TRAVEL_TRACKING)
  target_compile_definitions(MalaSimCore PUBLIC ENABLE_TRAVEL_TRACKING)
endif()

# include_directories(
#   ${PROJECT_SOURCE_DIR}/src
# )

# craete source files
# Add source files for the core library
file(GLOB_RECURSE MALASIM_CORE_SOURCES
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/*.hxx"
  "${PROJECT_SOURCE_DIR}/src/*.h"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/malasim/version_generated.h")
  message(STATUS "Using generated version_generated.h")
else()
  message(STATUS "Using default version.h")
endif()


# remove malasim/main.cpp from list
list(FILTER MALASIM_CORE_SOURCES EXCLUDE REGEX "${PROJECT_SOURCE_DIR}/src/malasim/main\\.cpp$")

# Add source files for the core library
add_library(MalaSimCore STATIC
    # Add your source files here
    ${MALASIM_CORE_SOURCES}
)
target_include_directories(MalaSimCore PUBLIC "${PROJECT_SOURCE_DIR}/src")

# Add coverage flags to MalaSimCore if enabled
if(ENABLE_COVERAGE)
  message(STATUS "LLVM Code coverage enabled for MalaSimCore")
  # Generate profile data and coverage mapping
  target_compile_options(MalaSimCore PRIVATE -fprofile-instr-generate -fcoverage-mapping)
  target_link_options(MalaSimCore PRIVATE -fprofile-instr-generate) # Might need link options too
endif()

# Link the core library and fmt to the main executable
target_link_libraries(MalaSimCore PRIVATE
  fmt::fmt-header-only
  GSL::gsl GSL::gslcblas
  yaml-cpp::yaml-cpp
  spdlog::spdlog
  date::date date::date-tz
  CLI11::CLI11
  unofficial::sqlite3::sqlite3
)

set_property(TARGET MalaSimCore PROPERTY CXX_STANDARD 20)

# Add the main executable
add_executable(MalaSim
    malasim/main.cpp
)

add_dependencies(MalaSim MalaSimCore)

if(ENABLE_COVERAGE)
  message(STATUS "LLVM Code coverage enabled for malasim_test")
  # Add flags for compiling the test executable itself
  target_compile_options(MalaSim PRIVATE -fprofile-instr-generate -fcoverage-mapping)
  # Add flags needed during linking
  target_link_options(MalaSim PRIVATE -fprofile-instr-generate)
endif()
# Link the core library and fmt to the main executable
target_link_libraries(MalaSim PRIVATE
  MalaSimCore
  fmt::fmt-header-only
  GSL::gsl GSL::gslcblas
  yaml-cpp::yaml-cpp
  spdlog::spdlog
  date::date date::date-tz
  CLI11::CLI11
  unofficial::sqlite3::sqlite3
)

set_property(TARGET MalaSim PROPERTY CXX_STANDARD 20)




