cmake_minimum_required(VERSION 3.10)

# Определение функции qt_add_executable
function(qt_add_executable TARGET_NAME)
    # Получаем все header и cpp файлы на текущем уровне каталога и ниже
    file(GLOB_RECURSE HEADER_FILES "*.h" "*.hpp")
    file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.cxx" "*.cc")

    # Добавляем исполняемый файл
    add_executable(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

    # Получаем остальные аргументы - список библиотек
    set(LIBS ${ARGN})

    # Цикл для добавления каждой библиотеки
    foreach(LIB ${LIBS})
        target_link_libraries(${TARGET_NAME} ${LIB})
    endforeach()

endfunction()
