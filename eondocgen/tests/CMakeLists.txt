cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonDocGenTests LANGUAGES CXX)
message(STATUS " - EonDocGenTests")

set(DOCGENTEST_HEADERS
	Regression.h
)
set(DOCGENTEST_SOURCES
	Regression.cpp
)

add_executable(EonDocGenTests ${DOCGENTEST_HEADERS} ${DOCGENTEST_SOURCES})
set_target_properties(
	EonDocGenTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_docgen_tests
		OUTPUT_NAME_RELEASE eon_docgen_tests
)

target_link_libraries(EonDocGenTests PUBLIC EonDocGen EonTest)
	
install(TARGETS EonDocGenTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonDocGenTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
