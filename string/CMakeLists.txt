cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(String LANGUAGES CXX)
message(STATUS " - String")

set(STRING_HEADERS
	String.h
	StringIterator.h
	Substring.h
	UniChar.h
)
set(STRING_SOURCES
	String.cpp
	StringIterator.cpp
	Substring.cpp
	UniChar.cpp
)

if(WIN32)
	set(STRING_OTHER string.natvis)
endif()

add_library(String STATIC ${STRING_HEADERS} ${STRING_SOURCES} ${STRING_OTHER})
set_target_properties(
	String
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_string${EON_DEBUG_POSTFIX}
		OUTPUT_NAME_RELEASE eon_string
)

install(TARGETS String
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${STRING_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/string")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/String.dir/Debug/String.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_INCLUDE_TESTS)
	add_subdirectory(tests)
endif()
