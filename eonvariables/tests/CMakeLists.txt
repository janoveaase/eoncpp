cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonVariablesTests LANGUAGES CXX)
message(STATUS " - EonVariablesTests")

set(VARIABLESTEST_HEADERS
	Regression.h
)
set(VARIABLESTEST_SOURCES
	Regression.cpp
)

add_executable(EonVariablesTests ${VARIABLESTEST_HEADERS} ${VARIABLESTEST_SOURCES})
set_target_properties(
	EonVariablesTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_variables_tests
		OUTPUT_NAME_RELEASE eon_variables_tests
)
set_property(TARGET EonVariablesTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonVariablesTests PUBLIC EonVariables EonTest)
	
install(TARGETS EonVariablesTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonVariablesTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
