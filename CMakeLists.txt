# Main Settings
cmake_minimum_required(VERSION 3.25)
project(CRASHGAME VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Testing
enable_testing()

# Slot
file(GLOB SLOT_SRC "include/slot/src/*.cpp")
file(GLOB SLOT_HDR "include/slot/include/*.h")

# Executable
add_executable(Main src/main.cpp 
    ${SLOT_SRC} ${SLOT_HDR})

target_include_directories(Main PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
)
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")


include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.10.0
)
FetchContent_MakeAvailable(Catch2)

# Tests
include(CTest)
include(Catch)

foreach(TEST_NAME namespace_tests winning_tests chacha_tests spin_tests game_tests)
    add_executable(${TEST_NAME} "test/${TEST_NAME}.cpp" ${SLOT_SRC} ${SLOT_HDR})
    target_include_directories(${TEST_NAME} PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/include"
    )
    target_link_libraries(${TEST_NAME} PRIVATE
    Catch2::Catch2WithMain
    )
    catch_discover_tests(${TEST_NAME} WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")
endforeach()


