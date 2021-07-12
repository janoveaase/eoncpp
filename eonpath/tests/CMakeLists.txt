cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonPathTests LANGUAGES CXX)
message(STATUS " - EonPathTests")

set(PATHTEST_HEADERS
	Regression.h
)
set(PATHTEST_SOURCES
	Regression.cpp
)

add_executable(EonPathTests ${PATHTEST_HEADERS} ${PATHTEST_SOURCES})
set_target_properties(
	EonPathTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_path_tests
		OUTPUT_NAME_RELEASE eon_path_tests
)
set_property(TARGET EonPathTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonPathTests PUBLIC EonPath EonTest)
	
install(TARGETS EonPathTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonPathTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
