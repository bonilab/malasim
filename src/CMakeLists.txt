find_package(fmt CONFIG REQUIRED)
find_package(GSL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)

# craete source files
# Add source files for the core library
file(GLOB_RECURSE MALASIM_CORE_SOURCES
  "*.cpp"
  "Core/Random/*.cpp"
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



