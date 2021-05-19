cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonFileTests LANGUAGES CXX)
message(STATUS " - EonFileTests")

set(FILETEST_HEADERS
	Regression.h
)
set(FILETEST_SOURCES
	Regression.cpp
)

add_executable(EonFileTests ${FILETEST_HEADERS} ${FILETEST_SOURCES})
set_target_properties(
	EonFileTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_file_tests
		OUTPUT_NAME_RELEASE eon_file_tests
)

target_link_libraries(EonFileTests PUBLIC EonFile EonTest)
	
install(TARGETS EonFileTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonFileTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
