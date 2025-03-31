file(GLOB SRCS *.cpp)
target_sources(3DGPProject PUBLIC ${SRCS})

if(USESDL)
    add_subdirectory(SDL2)
endif()