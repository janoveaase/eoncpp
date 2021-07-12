cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonParserTests LANGUAGES CXX)
message(STATUS " - EonParserTests")

set(PARSERTEST_HEADERS
	Regression.h
)
set(PARSERTEST_SOURCES
	Regression.cpp
)

add_executable(EonParserTests ${PARSERTEST_HEADERS} ${PARSERTEST_SOURCES})
set_target_properties(
	EonParserTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_parser_tests
		OUTPUT_NAME_RELEASE eon_parser_tests
)
set_property(TARGET EonParserTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonParserTests PUBLIC EonParser EonTest)
	
install(TARGETS EonParserTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonParserTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
