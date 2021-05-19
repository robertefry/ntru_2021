
cmake_minimum_required(VERSION 3.0)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
# CMAKE ENVIRONMENT
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

# Define project environment
project("NTRUX" C CXX)
enable_testing()

# Define code standards
set(CMAKE_C_STANDARD   "99")
set(CMAKE_CXX_STANDARD "20")

# Define compiler/linker flags
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
)
    set ("CMAKE_CXX_FLAGS" "-Wall -Wextra -std=c++20 -ggdb -pthread")
    set ("CMAKE_C_FLAGS"   "-Wall -Wextra -std=c++20 -ggdb -pthread")
endif()

# Define include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include mathpp
include_directories(mathpp/include)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
# PROJECT MAIN
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

set(PROJECT_MAIN_BINARY_NAME "ntrux")

# Glob sources & executible
file(GLOB_RECURSE PROJECT_MAIN_SOURCES
    "src/*.h" "src/*.hh" "src/*.hpp" "src/*.hxx"
    "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx"
)
add_executable(${PROJECT_MAIN_BINARY_NAME} ${PROJECT_MAIN_SOURCES})

# Add target `run_main`
file(RELATIVE_PATH "PROJECT_MAIN_BINARY_RELATIVE" ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_MAIN_BINARY_NAME})
add_custom_target(run_main
    COMMENT "Executing the '${PROJECT_MAIN_BINARY_NAME}' binary (${PROJECT_MAIN_BINARY_RELATIVE})..."
    COMMAND "${PROJECT_MAIN_BINARY_NAME}")
add_dependencies(run_main ${PROJECT_MAIN_BINARY_NAME})

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
# PROJECT MODEL
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

set(PROJECT_MODEL_BINARY_NAME "ntrux_model")

# Glob sources & executible
file(GLOB_RECURSE PROJECT_MODEL_SOURCES
    "model/*.h" "model/*.hh" "model/*.hpp" "model/*.hxx"
    "model/*.c" "model/*.cc" "model/*.cpp" "model/*.cxx"
)
add_executable(${PROJECT_MODEL_BINARY_NAME} ${PROJECT_MODEL_SOURCES})

# Add target `run_model`
file(RELATIVE_PATH "PROJECT_MODEL_BINARY_RELATIVE" ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_MODEL_BINARY_NAME})
add_custom_target(run_model
    COMMENT "Executing the '${PROJECT_MODEL_BINARY_NAME}' binary (${PROJECT_MODEL_BINARY_RELATIVE})..."
    COMMAND "${PROJECT_MODEL_BINARY_NAME}")
add_dependencies(run_model ${PROJECT_MODEL_BINARY_NAME})

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
# PROJECT TEST
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

set(PROJECT_TEST_BINARY_NAME "ntrux_tests")

# Include googletest
add_subdirectory(googletest)
include_directories(googletest/googletest/include)
include_directories(googletest/googlemock/include)

# Glob sources & executible
file(GLOB_RECURSE PROJECT_TEST_SOURCES
    "test/*.h" "test/*.hh" "test/*.hpp" "test/*.hxx"
    "test/*.c" "test/*.cc" "test/*.cpp" "test/*.cxx"
)
add_executable(${PROJECT_TEST_BINARY_NAME} ${PROJECT_TEST_SOURCES})

# Link tests with googletest
target_link_libraries(${PROJECT_TEST_BINARY_NAME} PUBLIC gtest_main gmock_main)
add_test(NAME ${PROJECT_TEST_BINARY_NAME} COMMAND ${PROJECT_TEST_BINARY_NAME})

# Add test executable as build target
add_custom_target(run_tests ALL COMMAND ctest DEPENDS ${PROJECT_TEST_BINARY_NAME})
add_custom_target(run_gtests COMMAND ${PROJECT_TEST_BINARY_NAME} DEPENDS ${PROJECT_TEST_BINARY_NAME})