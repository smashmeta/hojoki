cmake_minimum_required(VERSION 3.23)

project(hojoki VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick)
find_package(spdlog CONFIG REQUIRED)

add_executable(hojoki
    src/main.cpp
    src/main_widget.cpp
    src/main_widget.h
)

target_link_libraries(hojoki PRIVATE Qt6::Widgets Qt6::Quick spdlog::spdlog)

include(CTest)
if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)

    add_executable(hojoki_tests
        tests/main_widget_test.cpp
        src/main_widget.cpp
        src/main_widget.h
    )
    target_link_libraries(hojoki_tests PRIVATE Qt6::Widgets GTest::gtest_main GTest::gmock)
    add_test(NAME hojoki_tests COMMAND hojoki_tests)
    set_tests_properties(hojoki_tests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()
