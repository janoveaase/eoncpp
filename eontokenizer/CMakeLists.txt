cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTokenizer LANGUAGES CXX)
message(STATUS " - EonTokenizer")

set(TOKENIZER_HEADERS
	Tokenizer.h
	Token.h
	ReTokenizer.h
	TokenParser.h
	TokenMatcher.h
)
set(TOKENIZER_SOURCES
	Tokenizer.cpp
	ReTokenizer.cpp
	TokenParser.cpp
	TokenMatcher.cpp
)

if(WIN32)
	set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(EonTokenizer STATIC ${TOKENIZER_HEADERS} ${TOKENIZER_SOURCES})
set_target_properties(
	EonTokenizer
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_tokenizer
		OUTPUT_NAME_RELEASE eon_tokenizer
)
set_property(TARGET EonTokenizer PROPERTY CXX_STANDARD 17)

target_link_libraries(EonTokenizer PUBLIC EonString EonSource EonRegex)
target_include_directories(EonTokenizer PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonTokenizer
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${TOKENIZER_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eontokenizer")
if(WIN32)
	install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}/Debug/eon_tokenizer${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
