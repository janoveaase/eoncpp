cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTupleTests LANGUAGES CXX)
message(STATUS " - EonTupleTests")

set(TUPLETEST_HEADERS
	Regression.h
)
set(TUPLETEST_SOURCES
	Regression.cpp
)

add_executable(EonTupleTests ${TUPLETEST_HEADERS} ${TUPLETEST_SOURCES})
set_target_properties(
	EonTupleTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_tuple_tests
		OUTPUT_NAME_RELEASE eon_tuple_tests
)
set_property(TARGET EonTupleTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonTupleTests PUBLIC EonTuple EonTest)
	
install(TARGETS EonTupleTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonTupleTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
