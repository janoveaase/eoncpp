cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonOF LANGUAGES CXX)
message(STATUS " - EonOF")

set(EOF_HEADERS
	Eof.h
	Parser.h
)
set(EOF_SOURCES
	Eof.cpp
	Parser.cpp
)

add_library(EonOF STATIC ${EOF_HEADERS} ${EOF_SOURCES})
set_target_properties(
	EonOF
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_of
		OUTPUT_NAME_RELEASE eon_of
)

target_link_libraries(EonOF PUBLIC EonString EonName EonExcept EonRegex EonTuple EonPath EonFile)
target_include_directories(EonOF PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonOF
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${EOF_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonof")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonOF.dir/Debug/EonOF.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
