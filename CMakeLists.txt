cmake_minimum_required(VERSION 3.23)

project(hojoki VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick Test)
find_package(spdlog CONFIG REQUIRED)

add_executable(hojoki
    src/main.cpp
    src/board_item.cpp
    src/board_item.h
    src/main_widget.cpp
    src/main_widget.h
    src/task_item.cpp
    src/task_item.h
)

target_link_libraries(hojoki PRIVATE Qt6::Widgets Qt6::Quick spdlog::spdlog)

if(WIN32)
    add_custom_command(TARGET hojoki POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:hojoki>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:Qt6::QWindowsIntegrationPlugin>"
            "$<TARGET_FILE_DIR:hojoki>/platforms"
        COMMENT "Deploying the Qt Windows platform plugin"
    )
endif()

include(CTest)
if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)

    add_executable(hojoki_tests
        tests/test_main.cpp
        tests/board_item_test.cpp
        tests/main_widget_test.cpp
        src/board_item.cpp
        src/board_item.h
        src/main_widget.cpp
        src/main_widget.h
        src/task_item.cpp
        src/task_item.h
    )
    target_link_libraries(hojoki_tests PRIVATE Qt6::Widgets Qt6::Test GTest::gtest GTest::gmock)
    if(WIN32)
        add_custom_command(TARGET hojoki_tests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:hojoki_tests>/platforms"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:Qt6::QOffscreenIntegrationPlugin>"
                "$<TARGET_FILE_DIR:hojoki_tests>/platforms"
            COMMENT "Deploying the Qt offscreen platform plugin for tests"
        )
    endif()
    add_test(NAME hojoki_tests COMMAND hojoki_tests)
    set_tests_properties(hojoki_tests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()
