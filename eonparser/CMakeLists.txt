cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonParser LANGUAGES CXX)
message(STATUS " - EonParser")

set(PARSER_HEADERS
	ParserState.h
	ExpressionParser.h
	BoolParser.h
	ByteParser.h
	CharParser.h
	IntParser.h
	FloatParser.h
	BytesParser.h
	StringParser.h
	RegexParser.h
	NamePathParser.h
	PathParser.h
	NameParser.h
	TupleParserBase.h
	TupleParser.h
	TypeTupleParser.h
	DirectConversion.h
	ActionCallParser.h
	ActionFinder.h
	EdfParser.h
	EdfParserData.h
)
set(PARSER_SOURCES
	ParserState.cpp
	ExpressionParser.cpp
	BoolParser.cpp
	ByteParser.cpp
	CharParser.cpp
	IntParser.cpp
	FloatParser.cpp
	BytesParser.cpp
	StringParser.cpp
	RegexParser.cpp
	NamePathParser.cpp
	PathParser.cpp
	NameParser.cpp
	TupleParserBase.cpp
	TupleParser.cpp
	TypeTupleParser.cpp
	DirectConversion.cpp
	ActionCallParser.cpp
	ActionFinder.cpp
	EdfParser.cpp
	EdfParserData.cpp
)

if(WIN32)
	set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(EonParser STATIC ${PARSER_HEADERS} ${PARSER_SOURCES})
set_target_properties(
	EonParser
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_parser
		OUTPUT_NAME_RELEASE eon_parser
)
set_property(TARGET EonParser PROPERTY CXX_STANDARD 17)

set(libraries
	EonString
	EonExcept
	EonRegex
	EonSource
	EonFileSys
	EonContainers
	EonTokenizer
	EonInlineTest
	EonExpression
	EonTypeSystem
)
target_link_libraries(EonParser PUBLIC ${libraries})
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
	install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}/Debug/eon_parser${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
if(EON_INLINE_TESTS)
	message(STATUS "Including Inline Tests!")
	add_executable(EonParserInlineTests ${PARSER_HEADERS} ${PARSER_SOURCES})
	target_compile_definitions(EonParserInlineTests PRIVATE EON_INLINE_TEST_PROJECT)
	set_target_properties(
		EonParserInlineTests
		PROPERTIES
			OUTPUT_NAME_DEBUG eon_parser_inlinetests
			OUTPUT_NAME_RELEASE eon_parser_inlinetests
	)
	set_property(TARGET EonParserInlineTests PROPERTY CXX_STANDARD 17)

	target_link_libraries(EonParserInlineTests PUBLIC ${libraries})

	install(TARGETS EonParserInlineTests
		RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
	)

	target_include_directories(EonParserInlineTests PUBLIC
		"${EON_SOURCES_DIR}"
		"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}"
	)
endif()
