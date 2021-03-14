cmake_minimum_required(VERSION 2.8)
project(projetss6_hex_9150 C)

# Setup C99 depending on CMake version
macro(use_c99)
	if (CMAKE_VERSION VERSION_LESS "3.1")
		if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
			set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
		endif ()
	else ()
		set(CMAKE_C_STANDARD 99)
	endif ()
endmacro(use_c99)

# Setup GSL path
if (NOT DEFINED GSL_PATH)
	find_package(GSL REQUIRED)
	set(GSL_PATH ${GSL_ROOT_DIR})
endif ()

include_directories(${GSL_PATH}/include)
link_directories(${GSL_PATH}/lib)

add_subdirectory(src)
add_subdirectory(test)
