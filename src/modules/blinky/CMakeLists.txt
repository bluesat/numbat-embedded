project(blinky)
cmake_minimum_required(VERSION 3.5)

find_package(catkin REQUIRED
        COMPONENTS ti tlsf)

catkin_package(
        INCLUDE_DIRS ./
        LIBRARIES blinky
        CATKIN_DEPENDS ti tlsf
)

include_directories(
        ${CMAKE_INCLUDE_PATH}
        ./
        ${catkin_INCLUDE_DIRS}
)

set(BUILD_TOOLS_DIR ../../../build-tools)
include(${BUILD_TOOLS_DIR}/module_template.cmake)

add_module(blinky 0 1 0 blinky.cpp)


