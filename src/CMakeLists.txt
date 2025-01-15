find_package(fmt CONFIG REQUIRED)
find_package(GSL REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(spdlog REQUIRED)
find_package(date CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(stduuid CONFIG REQUIRED)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

# craete source files
# Add source files for the core library
file(GLOB_RECURSE MALASIM_CORE_SOURCES
  "*.cpp"
  "*.hxx"
  "*.h"
)

# Add source files for the core library
add_library(MalaSimCore STATIC
    # Add your source files here
    ${MALASIM_CORE_SOURCES}
)

# Link the core library and fmt to the main executable
target_link_libraries(MalaSimCore PUBLIC
  fmt::fmt
  GSL::gsl GSL::gslcblas
  yaml-cpp::yaml-cpp
  sol2
  spdlog::spdlog
  date::date date::date-tz
  CLI11::CLI11
  stduuid
)

set_property(TARGET MalaSimCore PROPERTY CXX_STANDARD 20)

# Add the main executable
add_executable(MalaSim
    Simulation/main.cpp
)

add_dependencies(MalaSim MalaSimCore)

# Link the core library and fmt to the main executable
target_link_libraries(MalaSim PRIVATE
  MalaSimCore
)

set_property(TARGET MalaSim PROPERTY CXX_STANDARD 20)



