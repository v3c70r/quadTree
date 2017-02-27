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
include_directories (${CMAKE_SOURCE_DIR}/src/world)
include_directories (${CMAKE_SOURCE_DIR}/src/thirdParty/imgui)

if(${ENABLE_TEST})
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif()

add_library(quadTree 
    ./src/quadTree/rectangle.cpp
    )
add_library(imgui
    ./src/thirdParty/imgui/imgui.cpp
    ./src/thirdParty/imgui/imgui_draw.cpp
    ./src/thirdParty/imgui/examples/opengl3_example/imgui_impl_glfw_gl3.cpp
    ./src/thirdParty/imgui/examples/opengl3_example/imgui_impl_glfw_gl3.cpp
    )
file(GLOB SRC "./src/mainWin.cpp")
add_executable(mainWin 
    ${SRC})
target_link_libraries(
    mainWin
    quadTree
    imgui
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${X11_LIBRARIES}
    ${CMAKE_DL_LIBS}
    )
