cmake_minimum_required(VERSION 3.10)

# Получаем имя корневой папки проекта
get_filename_component(FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Устанавливаем имя проекта
project(${FILE_NAME})

include(cmake-helper/cmake-helper.cmake)

use_qt5()

add_subdirectory(src)


