cmake_minimum_required(VERSION 3.16)

project(YadroTaskTests)

find_package(GTest REQUIRED)
include(GoogleTest)

add_library(TaskLibrary STATIC
    ${CMAKE_SOURCE_DIR}/src/FileParser.cpp
    ${CMAKE_SOURCE_DIR}/src/MyTime.cpp
    ${CMAKE_SOURCE_DIR}/src/ComputerClub.cpp
    ${CMAKE_SOURCE_DIR}/src/Validator.cpp
)

target_include_directories(TaskLibrary PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(YadroTaskTests
    main.cpp
    FileParserTest.cpp
    time_test.cpp
    ValidatorTest.cpp
)

target_include_directories(YadroTaskTests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(YadroTaskTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main 
    TaskLibrary
)

enable_testing()
gtest_discover_tests(YadroTaskTests)

add_custom_target(run_tests ALL
    COMMAND $<TARGET_FILE:YadroTaskTests>
    DEPENDS YadroTaskTests
    COMMENT "Running tests..."
    USES_TERMINAL
)