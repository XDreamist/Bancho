cmake_minimum_required(VERSION 3.22.0)
project(Bancho VERSION 0.1.0 LANGUAGES C CXX)

add_executable(
    Bancho 
    main.cpp 
    network/BanchoServer.cpp 
    network/BanchoServer.h
)

set_target_properties(Bancho PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED YES)

include(CTest)
enable_testing()