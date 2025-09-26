# Main Settings
cmake_minimum_required(VERSION 3.25)
project(CRASHGAME VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Testing
enable_testing()

# Crash Game
file(GLOB CRASHGAME_SRC "include/crashgame/src/*.cpp")
file(GLOB CRASHGAME_HDR "include/crashgame/include/*.h")

# Crash Game Utils
file(GLOB CRASHGAMEUTILS_SRC "include/crashgameutils/src/*.cpp")
file(GLOB CRASHGAMEUTILS_HDR "include/crashgameutils/include/*.h")

# Test
file(GLOB TARGET_TEST "test/*.cpp")

# Executable
add_executable(Main src/main.cpp 
    ${CRASHGAME_SRC} ${CRASHGAME_HDR}
    ${CRASHGAMEUTILS_SRC} ${CRASHGAMEUTILS_HDR})

target_include_directories(Main PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
)
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")

# No libraries to link

# Tests
add_executable(Test ${TARGET_TEST} 
    ${CRASHGAME_SRC} ${CRASHGAME_HDR}
    ${CRASHGAMEUTILS_SRC} ${CRASHGAMEUTILS_HDR})

target_include_directories(Test PUBLIC
    "${PROJECT_BINARY_DIR}"  
    "${PROJECT_SOURCE_DIR}/include"
)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.10.0
)
FetchContent_MakeAvailable(Catch2)

# Link Libraries
target_link_libraries(Test PRIVATE
    Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(Test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")

