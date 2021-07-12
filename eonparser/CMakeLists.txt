cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonParser LANGUAGES CXX)
message(STATUS " - EonParser")

set(PARSER_HEADERS
	Parser.h
	DocNode.h
)
set(PARSER_SOURCES
	Parser.cpp
	DocNode.cpp
)

add_library(EonParser STATIC ${PARSER_HEADERS} ${PARSER_SOURCES})
set_target_properties(
	EonParser
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_parser
		OUTPUT_NAME_RELEASE eon_parser
)
set_property(TARGET EonParser PROPERTY CXX_STANDARD 17)

target_link_libraries(EonParser PUBLIC
	EonString EonSource EonTokenizer EonRegex EonExcept EonName EonPath EonOF
	EonFile)
target_include_directories(EonParser PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonParser
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${PARSER_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonparser")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonParser.dir/Debug/EonParser.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()

add_executable(EonParserApp main.cpp)
target_link_libraries(EonParserApp EonParser)
set_target_properties(EonParserApp PROPERTIES OUTPUT_NAME "eonparser")
