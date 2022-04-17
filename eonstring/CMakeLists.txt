cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonString LANGUAGES CXX)
message(STATUS " - EonString")

set(STRING_HEADERS
	String.h
	StringIterator.h
	Substring.h
	UniChar.h
	NameDefs.h
	Name.h
	DefaultNames.h
	NamePath.h
	Hex.h
	Serializer.h
	ByteSerializer.h
	Stringifier.h
)
set(STRING_SOURCES
	String.cpp
	StringIterator.cpp
	Substring.cpp
	UniChar.cpp
	Name.cpp
	DefaultNames.cpp
	NamePath.cpp
	Hex.cpp
	Serializer.cpp
	Stringifier.cpp
)

if(WIN32)
	set(STRING_OTHER string.natvis name.natvis)
endif()

add_library(EonString STATIC ${STRING_HEADERS} ${STRING_SOURCES} ${STRING_OTHER})
set_target_properties(
	EonString
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_string
		OUTPUT_NAME_RELEASE eon_string
)
set_property(TARGET EonString PROPERTY CXX_STANDARD 17)

target_link_libraries(EonString PUBLIC EonExcept)
target_include_directories(EonString PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonString
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${STRING_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonstring")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonString.dir/Debug/EonString.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
