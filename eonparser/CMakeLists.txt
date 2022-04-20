cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonParser LANGUAGES CXX)
message(STATUS " - EonParser")

set(PARSER_HEADERS
	RawParser.h
	Element.h
	Parser.h
	OperatorRules.h
	BoolLiteralRule.h
	NamePathRule.h
	NumericalRules.h
	DefineRules.h
	ParseExpression.h
	ToolBox.h
)
set(PARSER_SOURCES
	RawParser.cpp
	Parser.cpp
	OperatorRules.cpp
	BoolLiteralRule.cpp
	NamePathRule.cpp
	NumericalRules.cpp
	DefineRules.cpp
	ParseExpression.cpp
	ToolBox.cpp
)

add_library(EonParser STATIC ${PARSER_HEADERS} ${PARSER_SOURCES})
set_target_properties(
	EonParser
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_parser
		OUTPUT_NAME_RELEASE eon_parser
)
set_property(TARGET EonParser PROPERTY CXX_STANDARD 17)

target_link_libraries(EonParser PUBLIC EonString
	EonSource EonTokenizer EonRegex EonExcept EonFileSys EonTypes
)
target_include_directories(EonParser PUBLIC "${EON_SOURCES_DIR}"
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}"
)

install(TARGETS EonParser
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${PARSER_HEADERS} DESTINATION
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonparser"
)
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/eon_parser_d.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()

#add_executable(EonParserApp main.cpp)
#target_link_libraries(EonParserApp EonParser)
#set_target_properties(EonParserApp PROPERTIES OUTPUT_NAME "eonparser")
