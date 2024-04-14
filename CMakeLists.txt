cmake_minimum_required(VERSION 3.10)

add_subdirectory(cmake-deps)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

project(Qt-polygon)


set(CMAKE_AUTOMOC ON)
set(QT_PACKAGES Core)
set(QT_LIBRARIES Qt5::Core)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

find_package(Qt5 COMPONENTS ${QT_PACKAGES} REQUIRED)

## load in pkg-config support
find_package(PkgConfig)

## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from above to find the location of libzmq
find_library(ZeroMQ_LIBRARY
            NAMES zmq
            PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
            )
            

add_subdirectory(src)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC ROOT_PATH="${CMAKE_SOURCE_DIR}/")
