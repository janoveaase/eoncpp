cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonSource LANGUAGES CXX)
message(STATUS " - EonSource")

set(SOURCE_HEADERS
	Raw.h
	File.h
	String.h
	SourceRef.h
	SourcePos.h
	SourceReporter.h
)
set(SOURCE_SOURCES
	File.cpp
	String.cpp
	SourceRef.cpp
	SourcePos.cpp
	SourceReporter.cpp
)

if(WIN32)
	set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(EonSource STATIC ${SOURCE_HEADERS} ${SOURCE_SOURCES})
set_target_properties(
	EonSource
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_source
		OUTPUT_NAME_RELEASE eon_source
)
set_property(TARGET EonSource PROPERTY CXX_STANDARD 17)

target_link_libraries(EonSource PUBLIC EonString EonExcept)
target_include_directories(EonSource PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonSource
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${SOURCE_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonsource")
if(WIN32)
	install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}/Debug/eon_source${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
if(EON_INLINE_TESTS)
	message(STATUS "Including Inline Tests!")
	add_executable(EonSourceInlineTests ${SOURCE_HEADERS} ${SOURCE_SOURCES})
	target_compile_definitions(EonSourceInlineTests PRIVATE EON_INLINE_TEST_PROJECT)
	set_target_properties(
		EonSourceInlineTests
		PROPERTIES
			OUTPUT_NAME_DEBUG eon_source_inlinetests
			OUTPUT_NAME_RELEASE eon_source_inlinetests
	)
	set_property(TARGET EonSourceInlineTests PROPERTY CXX_STANDARD 17)

	target_link_libraries(EonSourceInlineTests PUBLIC EonInlineTest)

	install(TARGETS EonSourceInlineTests
		RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
	)

	target_include_directories(EonSourceInlineTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
endif()
