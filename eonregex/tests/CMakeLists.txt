cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonRegexTests LANGUAGES CXX)
message(STATUS " - EonRegexTests")

set(REGEXTEST_HEADERS
	Regression.h
)
set(REGEXTEST_SOURCES
	Regression.cpp
)

add_executable(EonRegexTests ${REGEXTEST_HEADERS} ${REGEXTEST_SOURCES})
set_target_properties(
	EonRegexTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_regex_tests
		OUTPUT_NAME_RELEASE eon_regex_tests
)
set_property(TARGET EonRegexTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonRegexTests PUBLIC EonRegex EonString EonTest)
	
install(TARGETS EonRegexTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonRegexTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
