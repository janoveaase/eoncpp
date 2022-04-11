cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonFileSysTests LANGUAGES CXX)
message(STATUS " - EonFileSysTests")

set(FILESYSTEST_HEADERS
	Regression.h
)
set(FILESYSTEST_SOURCES
	Regression.cpp
)

add_executable(EonFileSysTests ${FILESYSTEST_HEADERS} ${FILESYSTEST_SOURCES})
set_target_properties(
	EonFileSysTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_filesys_tests
		OUTPUT_NAME_RELEASE eon_filesys_tests
)

target_link_libraries(EonFileSysTests PUBLIC EonFileSys EonTest EonRegex)
	
install(TARGETS EonFileSysTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonFileSysTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
