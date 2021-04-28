cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTokenizer LANGUAGES CXX)
message(STATUS " - EonTokenizer")

set(TOKENIZER_HEADERS
	Tokenizer.h
	Token.h
	TokenParser.h
)
set(TOKENIZER_SOURCES
	Tokenizer.cpp
	TokenParser.cpp
)

add_library(EonTokenizer STATIC ${TOKENIZER_HEADERS} ${TOKENIZER_SOURCES})
set_target_properties(
	EonTokenizer
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_tokenizer
		OUTPUT_NAME_RELEASE eon_tokenizer
)

target_link_libraries(EonTokenizer PUBLIC EonString EonSource)
target_include_directories(EonTokenizer PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonTokenizer
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${TOKENIZER_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eontokenizer")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonTokenizer.dir/Debug/EonTokenizer.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_INCLUDE_TESTS)
	add_subdirectory(tests)
endif()
