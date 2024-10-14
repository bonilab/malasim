find_package(fmt CONFIG REQUIRED)
find_package(GSL REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(spdlog REQUIRED)
find_package(date CONFIG REQUIRED)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${LUA_INCLUDE_DIR}
)

# craete source files
# Add source files for the core library
file(GLOB_RECURSE MALASIM_CORE_SOURCES
  "*.cpp"
  "Configuration/*.cpp"
  "Utils/*.cpp"
)

# Add source files for the core library
add_library(MalaSimCore STATIC
    # Add your source files here
    ${MALASIM_CORE_SOURCES}
        Configuration/SimulationTimeFrame.h
        Configuration/SpatialSettings.h
        Configuration/SeasonalitySettings.h
        Configuration/MovementSettings.h
        Configuration/ParasiteParameters.h
        Configuration/ImmuneSystemParameters.h
        Configuration/EpidemiologicalParameters.h
)

# Link the core library and fmt to the main executable
target_link_libraries(MalaSimCore PUBLIC
  fmt::fmt
  GSL::gsl GSL::gslcblas
  yaml-cpp::yaml-cpp
  ${LUA_LIBRARIES}
  sol2
  spdlog::spdlog
  date::date date::date-tz
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



