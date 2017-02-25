cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "-std=c++14 -g")

find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(OpenGL REQUIRED)
option(ENABLE_TEST off)

if (LINUX)
    find_package(X11 REQUIRED)
    include_directories(X11_INCLUDE_DIR)
endif()

include_directories (
    ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${OpenGL_INCLUDE_DIR})
include_directories (${CMAKE_SOURCE_DIR}/src/quadTree)

if(${ENABLE_TEST})
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif()

add_library(quadTree 
    ./src/quadTree/rectangle.cpp
    ./src/quadTree/quad.cpp
    )
file(GLOB SRC "./src/mainWin.cpp")
add_executable(mainWin 
    ${SRC})
target_link_libraries(
    mainWin
    quadTree
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${X11_LIBRARIES}
    ${CMAKE_DL_LIBS}
    )
