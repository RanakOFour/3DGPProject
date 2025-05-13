CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(3DGPProject)

add_executable(3DGPProject src/main.cpp)

add_subdirectory(src)

target_include_directories(3DGPProject PUBLIC include)
target_link_directories(3DGPProject PUBLIC lib)

if(WIN32)
	target_link_libraries(3DGPProject PUBLIC SDL2 SDL2main opengl32 glew32 tbb)
else()
	target_link_libraries(3DGPProject PUBLIC SDL2 SDL2main GL GLEW tbb)
endif()


# Move .dll files to build
add_custom_command(TARGET 3DGPProject POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   "${PROJECT_SOURCE_DIR}/bin"
				   $<TARGET_FILE_DIR:3DGPProject>)

add_custom_command(TARGET 3DGPProject POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   "${PROJECT_SOURCE_DIR}/resources"
				   $<TARGET_FILE_DIR:3DGPProject>/resources/)