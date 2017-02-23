cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "-std=c++14 -g")

find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(OpenGL REQUIRED)

if (LINUX)
    find_package(X11 REQUIRED)
    include_directories(X11_INCLUDE_DIR)
endif()

include_directories ( ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${OpenGL_INCLUDE_DIR})
file(GLOB SRC "src/*.cpp")
add_executable(mainWin 
    ${SRC})
target_link_libraries(
    mainWin
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${X11_LIBRARIES}
    ${CMAKE_DL_LIBS}
    )
