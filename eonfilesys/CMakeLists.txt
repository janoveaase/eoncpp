cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonFileSys LANGUAGES CXX)
message(STATUS " - EonFileSys")

set(FILESYS_HEADERS
	FileSys.h
	Path.h
	File.h
	Dir.h
	FileBuffer.h
)
set(FILESYS_SOURCES
	Path.cpp
	File.cpp
	Dir.cpp
	FileBuffer.cpp
)

if(WIN32)
	set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(EonFileSys STATIC ${FILESYS_HEADERS} ${FILESYS_SOURCES})
set_target_properties(
	EonFileSys
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_filesys
		OUTPUT_NAME_RELEASE eon_filesys
)
set_property(TARGET EonFileSys PROPERTY CXX_STANDARD 17)

target_link_libraries(EonFileSys PUBLIC EonString EonExcept)
target_include_directories(EonFileSys PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonFileSys
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${FILESYS_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonfilesys")
if(WIN32)
	install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}/Debug/eon_filesys${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
if(EON_INLINE_TESTS)
	message(STATUS "Including Inline Tests!")
	add_executable(EonFilesysInlineTests ${FILESYS_HEADERS} ${FILESYS_SOURCES})
	target_compile_definitions(EonFilesysInlineTests PRIVATE EON_INLINE_TEST_PROJECT)
	set_target_properties(
		EonFilesysInlineTests
		PROPERTIES
			OUTPUT_NAME_DEBUG eon_filesys_inlinetests
			OUTPUT_NAME_RELEASE eon_filesys_inlinetests
	)
	set_property(TARGET EonFilesysInlineTests PROPERTY CXX_STANDARD 17)

	target_link_libraries(EonFilesysInlineTests PUBLIC EonInlineTest)

	install(TARGETS EonFilesysInlineTests
		RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
	)

	target_include_directories(EonFilesysInlineTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
endif()
