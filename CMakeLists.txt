cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(Eon LANGUAGES CXX)
set(VERSION "0.1.1")
message(STATUS "Eon C++ v${VERSION}")

# Configuration
set(EON_SOURCES_DIR "${CMAKE_SOURCE_DIR}" CACHE PATH "The directory containing the source files")
set(EON_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Where to install")
set(EON_LIBRARY_DIR "lib" CACHE STRING "Where to install libraries under EON_INSTALL_DIR")
set(EON_INCLUDE_DIR "include" CACHE STRING "Where to install headers under EON_INSTALL_DIR")
set(EON_DEBUG_POSTFIX "_d" CACHE STRING "Postfix to debug builds")
set(EON_INCLUDE_TESTS ON CACHE BOOL "Include tests in build?")
set(EON_TESTS_DIR "tests" CACHE PATH "Where to install tests under EON_INSTALL_DIR")

if(EON_INCLUDE_TESTS)
	add_subdirectory(test)
endif()
add_subdirectory(string)
