cmake_minimum_required(VERSION 3.1)

project(lyn VERSION "2.5.3")

option(USE_STATIC_LIBRARIES "When on, passes -static to the compiler" ON)

include(config.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(ELFCPP_SOURCE_LIST
	elfcpp/aarch64.h
	elfcpp/dwarf.h
	elfcpp/elfcpp.h
	elfcpp/elfcpp_swap.h
	elfcpp/mips.h
	elfcpp/sparc.h
	elfcpp/x86_64.h
	elfcpp/arm.h
	elfcpp/elfcpp_file.h
	elfcpp/elfcpp_internal.h
	elfcpp/i386.h
	elfcpp/powerpc.h
	elfcpp/s390.h
	elfcpp/tilegx.h
)

set(LYN_SOURCE_LIST
	main.cpp

	util/hex_write.h

	core/data_chunk.h
	core/data_chunk.cpp

	core/data_file.h
	core/data_file.cpp

	ea/event_code.h
	ea/event_code.cpp

	ea/event_section.h
	ea/event_section.cpp

	core/section_data.h
	core/section_data.cpp

	core/event_object.h
	core/event_object.cpp

	core/arm_relocator.h
	core/arm_relocator.cpp
)

set(SOURCE_LIST
	${ELFCPP_SOURCE_LIST}
	${LYN_SOURCE_LIST}
)

# this is to ensure inclusion relative to base directory is allowed
include_directories(${CMAKE_SOURCE_DIR})

if(USE_STATIC_LIBRARIES)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
endif()

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
