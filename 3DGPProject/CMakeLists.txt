CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

project(3DGPProject)

add_executable(3DGPProject main.cpp)

add_subdirectory(src)

target_include_directories(3DGPProject PUBLIC include)
target_link_directories(3DGPProject PUBLIC lib)

if(WIN32)
	target_link_libraries(3DGPProject PUBLIC SDL2 SDL2main opengl32 glew32)
else()
	target_link_libraries(3DGPProject PUBLIC SDL2 SDL2main GL GLEW)
endif()
