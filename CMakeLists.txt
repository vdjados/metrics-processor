cmake_minimum_required(VERSION 3.10)

project(MetricsCollector CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add preprocessor definition for MSVC to suppress localtime warning
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Include the metrics directory
include_directories(metrics)

# Add the executable
add_executable(metrics_app main.cpp metrics/MetricsCollector.cpp)

# Link necessary libraries (e.g., pthreads for Linux/macOS, not strictly needed for MSVC if using default threading model)
target_link_libraries(metrics_app 
    # For Windows, no explicit pthreads link is typically needed with MSVC
    # For Linux/macOS, uncomment the line below if threading issues occur
    # pthread
) 