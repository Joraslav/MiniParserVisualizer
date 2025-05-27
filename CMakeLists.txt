cmake_minimum_required(VERSION 3.28.0)
project(MiniParserVisualizer VERSION 0.1.0 LANGUAGES CXX)

message(STATUS "Version ${PROJECT_NAME}: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Wall
    -Wpedantic
    -Wconversion
)

include_directories(${CMAKE_SOURCE_DIR}/src)

# GTest
find_package(GTest REQUIRED)
if(NOT GTest_FOUND)
    message(STATUS "GTest not found. Trying to download...")

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.12.1.tar.gz
    )
    FetchContent_MakeAvailable(googletest)
    find_package(GTest REQUIRED)
    if(GTest_FOUND)
        message(STATUS "GTest downloaded and found ^_^. Version: ${GTest_VERSION}")
    else()
        message(FATAL_ERROR "GTest not found after download")
    endif()

else()
    message(STATUS "GTest found ^_^. Version: ${GTest_VERSION}")
endif()

# nlohmann/json
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)
include_directories(${nlohmann_json_SOURCE_DIR}/include)
message(STATUS "nlohmann_json downloaded and included ^_^.")

# My libraries
add_library(Parser STATIC
    src/Parser.cpp
)
target_link_libraries(Parser PRIVATE nlohmann_json::nlohmann_json)

# Executable for data
add_executable(data_generator src/Generator.cpp)

# Main executable
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Parser)

include(CTest)
enable_testing()

