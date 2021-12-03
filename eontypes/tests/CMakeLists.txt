cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTypesTests LANGUAGES CXX)
message(STATUS " - EonTypesTests")

set(TYPESTEST_HEADERS
	Regression.h
)
set(TYPESTEST_SOURCES
	Regression.cpp
)

add_executable(EonTypesTests ${TYPESTEST_HEADERS} ${TYPESTEST_SOURCES})
set_target_properties(
	EonTypesTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_types_tests
		OUTPUT_NAME_RELEASE eon_types_tests
)
set_property(TARGET EonTypesTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonTypesTests PUBLIC EonTypes EonTest)
	
install(TARGETS EonTypesTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonTypesTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
