cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonDocTests LANGUAGES CXX)
message(STATUS " - EonDocTests")

set(EDOCTEST_HEADERS
	Regression.h
)
set(EDOCTEST_SOURCES
	Regression.cpp
)

add_executable(EonDocTests ${EDOCTEST_HEADERS} ${EDOCTEST_SOURCES})
set_target_properties(
	EonDocTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_doc_tests
		OUTPUT_NAME_RELEASE eon_doc_tests
)

target_link_libraries(EonDocTests PUBLIC EonDoc EonTest)
	
install(TARGETS EonDocTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonDocTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
