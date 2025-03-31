file(GLOB SRCS *.cpp)
target_sources(3DGPProject PUBLIC ${SRCS})

add_subdirectory(Entity)
add_subdirectory(Scene)