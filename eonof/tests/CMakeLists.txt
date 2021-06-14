cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTFTests LANGUAGES CXX)
message(STATUS " - EonTFTests")

set(ETFTEST_HEADERS
	Regression.h
)
set(ETFTEST_SOURCES
	Regression.cpp
)

add_executable(EonTFTests ${ETFTEST_HEADERS} ${ETFTEST_SOURCES})
set_target_properties(
	EonTFTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_tf_tests
		OUTPUT_NAME_RELEASE eon_tf_tests
)

target_link_libraries(EonTFTests PUBLIC EonTF EonTest)
	
install(TARGETS EonTFTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonTFTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
