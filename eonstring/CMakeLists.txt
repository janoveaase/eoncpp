cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonString LANGUAGES CXX)
message(STATUS " - EonString")

set(STRING_HEADERS
	Locale.h
	String.h
	StringIterator.h
	Substring.h
	UniChar.h
	NameDefs.h
	Name.h
	NameData.h
	DefaultNames.h
	NamePath.h
	Hex.h
	Serializer.h
	ByteSerializer.h
	Stringifier.h
	StringifierDefs.h
	GlobPattern.h
)
set(STRING_SOURCES
	Locale.cpp
	String.cpp
	StringAsNumber.cpp
	StringAssign.cpp
	StringConstruct.cpp
	StringCopyTransform.cpp
	StringIteration.cpp
	StringModifying.cpp
	StringStatic.cpp
	StringSearching.cpp
	StringIterator.cpp
	Substring.cpp
	SubstringReadOnly.cpp
	SubstringAsNumber.cpp
	SubstringComparing.cpp
	SubstringSearching.cpp
	UniChar.cpp
	Name.cpp
	NameData.cpp
	DefaultNames.cpp
	NamePath.cpp
	Hex.cpp
	Serializer.cpp
	Stringifier.cpp
	GlobPattern.cpp
)

if(WIN32)
	set(STRING_OTHER string.natvis name.natvis)
	set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
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
install(FILES ${STRING_HEADERS} ${STRING_OTHER} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonstring")
if(WIN32)
	install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}/Debug/eon_string${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
if(EON_INLINE_TESTS)
	message(STATUS "Including Inline Tests!")
	add_executable(EonStringInlineTests ${STRING_HEADERS} ${STRING_SOURCES})
	target_compile_definitions(EonStringInlineTests PRIVATE EON_INLINE_TEST_PROJECT)
	set_target_properties(
		EonStringInlineTests
		PROPERTIES
			OUTPUT_NAME_DEBUG eon_string_inlinetests
			OUTPUT_NAME_RELEASE eon_string_inlinetests
	)
	set_property(TARGET EonStringInlineTests PROPERTY CXX_STANDARD 17)

	target_link_libraries(EonStringInlineTests PUBLIC EonInlineTest)

	install(TARGETS EonStringInlineTests
		RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
	)

	target_include_directories(EonStringInlineTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
endif()
