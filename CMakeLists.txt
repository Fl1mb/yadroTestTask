cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(YadroTestTask CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib)

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/out/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/out/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/out/lib)
endforeach()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=1")

set(SOURCES
    src/main.cpp
    src/FileParser.cpp
    src/MyTime.cpp
    src/ComputerClub.cpp
    src/Validator.cpp
)

set(HEADERS
    src/FileParser.h
    src/Event.h
    src/ComputerClub.h
    src/MyTime.h
    src/Validator.h
)

add_executable(computer_club
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(computer_club PRIVATE src)

if(MSVC)
    target_compile_options(computer_club PRIVATE /W4)
else()
    target_compile_options(computer_club PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(BUILD_TESTS "Build tests" Off)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_custom_target(clean_out
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/out
    COMMENT "Cleaning output directory"
)