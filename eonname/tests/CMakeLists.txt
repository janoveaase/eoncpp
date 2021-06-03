cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonNameTests LANGUAGES CXX)
message(STATUS " - EonNameTests")

set(NAMETEST_HEADERS
	Regression.h
)
set(NAMETEST_SOURCES
	Regression.cpp
)

add_executable(EonNameTests ${NAMETEST_HEADERS} ${NAMETEST_SOURCES})
set_target_properties(
	EonNameTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_name_tests
		OUTPUT_NAME_RELEASE eon_name_tests
)

target_link_libraries(EonNameTests PUBLIC EonName EonTest)
	
install(TARGETS EonNameTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonNameTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
