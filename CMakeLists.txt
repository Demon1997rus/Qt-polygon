cmake_minimum_required(VERSION 3.10)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

project(Qt-polygon)

set(CMAKE_AUTOMOC ON)
set(QT_PACKAGES Core)
set(QT_LIBRARIES Qt5::Core)

find_package(Qt5 COMPONENTS ${QT_PACKAGES} REQUIRED)

add_subdirectory(src)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC ROOT_PATH="${CMAKE_SOURCE_DIR}/")
