cmake_minimum_required(VERSION 3.1)

project(ea-dep)

option(USE_STATIC_LIBRARIES "When on, passes -static to the compiler" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

find_package(RE2C REQUIRED)

RE2C_TARGET(
	NAME IncludeParser

	INPUT   "${CMAKE_CURRENT_SOURCE_DIR}/parser.repp"
	OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/parser.h"
)

# this is to ensure inclusion relative to base directory is allowed
include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIST
    "main.cpp"

    "file-utils.h"

    "parser.h"
    "parser.cpp"
)

if(USE_STATIC_LIBRARIES)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
endif()

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
