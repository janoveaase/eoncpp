cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonOFTests LANGUAGES CXX)
message(STATUS " - EonOFTests")

set(EOFTEST_HEADERS
	Regression.h
)
set(EOFTEST_SOURCES
	Regression.cpp
)

add_executable(EonOFTests ${EOFTEST_HEADERS} ${EOFTEST_SOURCES})
set_target_properties(
	EonOFTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_of_tests
		OUTPUT_NAME_RELEASE eon_of_tests
)

target_link_libraries(EonOFTests PUBLIC EonOF EonTest)
	
install(TARGETS EonOFTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonOFTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
