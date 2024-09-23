find_package(fmt CONFIG REQUIRED)

# Add source files for the core library
add_library(MalaSimCore STATIC
    # Add your source files here
    example.cpp
)

# Add the main executable
add_executable(MalaSim
    main.cpp
)

# Link the core library and fmt to the main executable
target_link_libraries(MalaSim PRIVATE
  MalaSimCore
  fmt::fmt
)



