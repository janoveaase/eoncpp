cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTerminal LANGUAGES CXX)
message(STATUS " - EonTerminal")

set(TERMINAL_HEADERS
	Terminal.h
)
set(TERMINAL_SOURCES
	Terminal.cpp
)

add_library(EonTerminal STATIC ${TERMINAL_HEADERS} ${TERMINAL_SOURCES})
set_target_properties(
	EonTerminal
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_terminal
		OUTPUT_NAME_RELEASE eon_terminal
)
set_property(TARGET EonTerminal PROPERTY CXX_STANDARD 17)

target_link_libraries(EonTerminal PUBLIC EonString EonName)
target_include_directories(EonTerminal PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonTerminal
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${TERMINAL_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonterminal")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonTerminal.dir/Debug/EonTerminal.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
