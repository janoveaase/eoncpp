cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonRegExTests LANGUAGES CXX)
message(STATUS " - EonRegExTests")

set(REGEXTEST_HEADERS
	Regression.h
)
set(REGEXTEST_SOURCES
	Regression.cpp
)

add_executable(EonRegExTests ${REGEXTEST_HEADERS} ${REGEXTEST_SOURCES})
set_target_properties(
	EonRegExTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_regex_tests
		OUTPUT_NAME_RELEASE eon_regex_tests
)

target_link_libraries(EonRegExTests PUBLIC EonRegEx EonString EonTest)
	
install(TARGETS EonRegExTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonRegExTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
