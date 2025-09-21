cmake_minimum_required(VERSION 3.22.0) # Later change to latest version like 3.23.0 if cmake is also latest
project(Bancho VERSION 0.1.0 LANGUAGES C CXX)

# Find SFML
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# Define the executable and its source files
add_executable(
    Bancho 
    main.cpp 
    network/BanchoServer.cpp 
    network/BanchoServer.h
)

# Set the C++ standard for the target
set_target_properties(Bancho PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED YES)

# Optional: Enable testing
include(CTest)
enable_testing()

# Link SFML
target_link_libraries(Bancho PRIVATE sfml-system sfml-window sfml-graphics)