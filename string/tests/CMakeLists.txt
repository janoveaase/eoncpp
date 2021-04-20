cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(StringTests LANGUAGES CXX)
message(STATUS " - StringTests")

set(STRINGTEST_HEADERS
	Regression.h
)
set(STRINGTEST_SOURCES
	Regression.cpp
)

add_executable(StringTests ${STRINGTEST_HEADERS} ${STRINGTEST_SOURCES})
set_target_properties(
	StringTests
	PROPERTIES
		OUTPUT_NAME_DEBUG string_tests${EON_DEBUG_POSTFIX}
		OUTPUT_NAME_RELEASE string_tests
)

target_link_libraries(StringTests PUBLIC String Test)
#target_link_libraries(StringTests PUBLIC optimized "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}/string" debug "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}/string${EON_DEBUG_POSTFIX}")
#target_link_libraries(StringTests PUBLIC optimized "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}/test" debug "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}/test${EON_DEBUG_POSTFIX}")
	
install(TARGETS StringTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories( StringTests PUBLIC "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}" "${CMAKE_SOURCE_DIR}")
