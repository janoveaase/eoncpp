cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonHexTests LANGUAGES CXX)
message(STATUS " - EonHexTests")

set(HEXTEST_HEADERS
	Regression.h
)
set(HEXTEST_SOURCES
	Regression.cpp
)

add_executable(EonHexTests ${HEXTEST_HEADERS} ${HEXTEST_SOURCES})
set_target_properties(
	EonHexTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_hex_tests
		OUTPUT_NAME_RELEASE eon_hex_tests
)

target_link_libraries(EonHexTests PUBLIC EonHex EonTest EonExcept)
	
install(TARGETS EonHexTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonHexTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
