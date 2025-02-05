cmake_minimum_required(VERSION 3.15...3.27)
project(cpython_bind VERSION 0.1.127)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------
# Compiler
# ------------------------------------
if(MSVC)
	set(CMAKE_C_COMPILER msvc)
	set(CMAKE_CXX_COMPILER msvc)
	#target_compile_options(<target> PUBLIC "/ZI")
	#target_link_options(<target> PUBLIC "/INCREMENTAL")
elseif()
	message( STATUS "Only MSVC compiler supported" )
endif()


if (CMAKE_VERSION VERSION_LESS 3.18)
	set(DEV_MODULE Development)
else()
	set(DEV_MODULE Development.Module)
endif()

# Find Python package
find_package(Python 3.9 COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message("" CMAKE_BUILD_TYPE)

# exporting .lib & .dll
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Detect the installed nanobind package and import it into CMake
execute_process(
	COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
	OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)
find_package(nanobind CONFIG REQUIRED)

# It will compile a new extension named my_ext from the source code contained in the file my_ext.cpp.
nanobind_add_module(
	# Name of the extension
	cpy

	# Don't perform optimizations to minimize binary size
	# this flag can be optimize for speed (-O3)
	NOMINSIZE

	# Perform link time optimization.
	#LTO

	# Don't strip unneded symbols and debug information from 
	# the compiled extension when performing release builds.
	#NOSTRIP
	
	# Target the stable ABI for Python 3.12+, which reduces
	# the number of binary wheels that must be built. This
	# does nothing on older Python versions
	#STABLE_ABI

	# Build libnanobind statically and merge it into the
	# extension (which itself remains a shared library)
	#
	# If your project builds multiple extensions, you can
	# replace this flag by NB_SHARED to conserve space by
	# reusing a shared libnanobind across libraries
	NB_STATIC

	# Source code goes here
	src/cpy.cpp
)

# Creating Executable Program for Python Interperter

# ----------------------------------------------
# Library Defination - utils ve c_object
# ----------------------------------------------
add_library(utils STATIC ${CMAKE_SOURCE_DIR}/src/utils.cc)

# Include directories
target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(cpy PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(cpy PRIVATE ${Python3_LIBRARIES})

# Compile Configuration
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(Python3_LIBRARY ${Python3_LIBRARY_DEBUG})
else()
		set(Python3_LIBRARY ${Python3_LIBRARY_RELEASE})
endif()
# Send debug flag to nanobind
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(cpy PRIVATE NB_DEBUG)
endif()

# ----------------------------------------------
# Source 
# ----------------------------------------------
target_sources(cpy PRIVATE 
	src/py_binding.cc
	include/py_binding.h
)
target_sources(utils PRIVATE
	src/utils.cc
	include/utils.h
)# Linking libraries
target_link_libraries(cpy PRIVATE utils)
