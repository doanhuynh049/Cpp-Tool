cmake_minimum_required(VERSION 3.12)
project(MyProject)

# Compiler settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Directories
set(SRC_DIR src)
set(TEST_DIR test)
set(BUILD_DIR build)
set(BIN_DIR bin)

# Source files
file(GLOB SRCS ${SRC_DIR}/*.cpp)
set(OBJS "")
foreach(SRC IN LISTS SRCS)
    get_filename_component(OBJ ${SRC} NAME_WE)
    set(OBJ_FILE ${BUILD_DIR}/${OBJ}.o)
    add_custom_command(
        OUTPUT ${OBJ_FILE}
        COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -c ${SRC} -o ${OBJ_FILE}
        DEPENDS ${SRC}
    )
    list(APPEND OBJS ${OBJ_FILE})
endforeach()

# Test source files
file(GLOB TEST_SRCS ${TEST_DIR}/*.cpp)
set(TEST_OBJS "")
foreach(TEST_SRC IN LISTS TEST_SRCS)
    get_filename_component(TEST_OBJ ${TEST_SRC} NAME_WE)
    set(TEST_OBJ_FILE ${BUILD_DIR}/${TEST_OBJ}.o)
    add_custom_command(
        OUTPUT ${TEST_OBJ_FILE}
        COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -c ${TEST_SRC} -o ${TEST_OBJ_FILE}
        DEPENDS ${TEST_SRC}
    )
    list(APPEND TEST_OBJS ${TEST_OBJ_FILE})
endforeach()

# Executable name
set(EXECUTABLE my_test)

# GTest libraries
set(GTEST_DIR /usr/src/gtest)
set(GTEST_INCLUDE ${GTEST_DIR}/include)
set(GTEST_LIB ${GTEST_DIR}/lib)
set(GTEST_MAIN ${GTEST_LIB}/gtest_main.a)

# Targets
add_executable(${EXECUTABLE} ${OBJS} ${TEST_OBJS})
target_include_directories(${EXECUTABLE} PRIVATE ${GTEST_INCLUDE})
target_link_libraries(${EXECUTABLE} PRIVATE ${GTEST_MAIN})

# Clean target
add_custom_target(my_clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove ${EXECUTABLE}
)