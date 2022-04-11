cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTerminalTests LANGUAGES CXX)
message(STATUS " - EonTerminalTests")

set(TERMINALTEST_HEADERS
	Regression.h
)
set(TERMINALTEST_SOURCES
	Regression.cpp
)

add_executable(EonTerminalTests ${TERMINALTEST_HEADERS} ${TERMINALTEST_SOURCES})
set_target_properties(
	EonTerminalTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_terminal_tests
		OUTPUT_NAME_RELEASE eon_terminal_tests
)
set_property(TARGET EonTerminalTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonTerminalTests PUBLIC EonTerminal EonTest EonRegex)
	
install(TARGETS EonTerminalTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonTerminalTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
