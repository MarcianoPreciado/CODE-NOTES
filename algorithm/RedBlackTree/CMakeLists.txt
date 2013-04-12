project(program)
cmake_minimum_required(VERSION 2.8)
SET( CMAKE_COLOR_MAKEFILE ON )
#enable warnings
IF( CMAKE_COMPILER_IS_GNUCXX )
  ADD_DEFINITIONS( -g -Wall)
ENDIF( CMAKE_COMPILER_IS_GNUCXX )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ./ )
SET( CMAKE_BUILD_TYPE Debug)

file (GLOB headers "${CMAKE_SOURCE_DIR}/*.h")
message("My headers: " ${headers})

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${headers})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
