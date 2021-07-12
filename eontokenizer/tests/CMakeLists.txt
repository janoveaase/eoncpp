cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTokenizerTests LANGUAGES CXX)
message(STATUS " - EonTokenizerTests")

set(TOKENIZERTEST_HEADERS
	Regression.h
)
set(TOKENIZERTEST_SOURCES
	Regression.cpp
)

add_executable(EonTokenizerTests ${TOKENIZERTEST_HEADERS} ${TOKENIZERTEST_SOURCES})
set_target_properties(
	EonTokenizerTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_tokenizer_tests
		OUTPUT_NAME_RELEASE eon_tokenizer_tests
)
set_property(TARGET EonTokenizerTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonTokenizerTests PUBLIC EonTokenizer EonTest)
	
install(TARGETS EonTokenizerTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonTokenizerTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
