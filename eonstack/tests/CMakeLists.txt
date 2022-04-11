cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonStackTests LANGUAGES CXX)
message(STATUS " - EonStackTests")

set(STACKTEST_HEADERS
	Regression.h
)
set(STACKTEST_SOURCES
	Regression.cpp
)

add_executable(EonStackTests ${STACKTEST_HEADERS} ${STACKTEST_SOURCES})
set_target_properties(
	EonStackTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_stack_tests
		OUTPUT_NAME_RELEASE eon_stack_tests
)
set_property(TARGET EonStackTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonStackTests PUBLIC EonStack EonTest EonExcept EonRegex)
	
install(TARGETS EonStackTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonStackTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
