cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonEdocTests LANGUAGES CXX)
message(STATUS " - EonEdocTests")

set(EDOCTEST_HEADERS
	Regression.h
)
set(EDOCTEST_SOURCES
	Regression.cpp
)

add_executable(EonEdocTests ${EDOCTEST_HEADERS} ${EDOCTEST_SOURCES})
set_target_properties(
	EonEdocTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_edoc_tests
		OUTPUT_NAME_RELEASE eon_edoc_tests
)
set_property(TARGET EonEdocTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonEdocTests PUBLIC EonEdoc EonTest EonRegex)
	
install(TARGETS EonEdocTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonEdocTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
