cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonSourceTests LANGUAGES CXX)
message(STATUS " - EonSourceTests")

set(SOURCETEST_HEADERS
	Regression.h
)
set(SOURCETEST_SOURCES
	Regression.cpp
)

add_executable(EonSourceTests ${SOURCETEST_HEADERS} ${SOURCETEST_SOURCES})
set_target_properties(
	EonSourceTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_source_tests
		OUTPUT_NAME_RELEASE eon_source_tests
)

target_link_libraries(EonSourceTests PUBLIC EonSource EonTest)
	
install(TARGETS EonSourceTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonSourceTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
