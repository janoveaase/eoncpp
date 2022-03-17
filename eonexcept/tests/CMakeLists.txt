cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonExceptTests LANGUAGES CXX)
message(STATUS " - EonExceptTests")

set(EXCEPTTEST_HEADERS
	Regression.h
)
set(EXCEPTTEST_SOURCES
	Regression.cpp
)

add_executable(EonExceptTests ${EXCEPTTEST_HEADERS} ${EXCEPTTEST_SOURCES})
set_target_properties(
	EonExceptTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_except_tests
		OUTPUT_NAME_RELEASE eon_except_tests
)
set_property(TARGET EonExceptTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonExceptTests PUBLIC EonString EonTest)
	
install(TARGETS EonExceptTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonExceptTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
