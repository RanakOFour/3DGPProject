CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

project(3DGPPrject)

add_executable(3DGPPrject main.cpp)

add_subdirectory(src)

target_include_directories(3DGPPrject PUBLIC include)
target_link_directories(3DGPPrject PUBLIC lib)

target_link_libraries(3DGPPrject PUBLIC SDL2 GL GLEW)