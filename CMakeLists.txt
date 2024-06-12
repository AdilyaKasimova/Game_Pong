cmake_minimum_required(VERSION 3.27)
project(Game_Pong)

set(CMAKE_CXX_STANDARD 17)

# Установка пути к локально распакованному GoogleTest
set(GOOGLETEST_ROOT "${CMAKE_SOURCE_DIR}/external/googletest-1.14.0")

add_subdirectory(${GOOGLETEST_ROOT} googletest)

# Подавление предупреждений о C++11 расширениях
add_compile_options(-Wno-c++11-extensions)

add_executable(Game_Pong src/win32_platform.cpp)

set(SOURCE_FILES src/win32_platform.cpp)

target_compile_features(Game_Pong PRIVATE cxx_std_17)

target_sources(Game_Pong PRIVATE src/win32_platform.cpp)

# Добавление тестового исполняемого файла

include_directories(Tests ${GOOGLETEST_ROOT}/googletest/include)
add_executable(
        Tests
        test/tests.cpp
)
target_link_libraries(Tests gtest gtest_main)

# Включение тестирования и добавление тестов
enable_testing()
add_test(NAME Tests COMMAND Tests)
