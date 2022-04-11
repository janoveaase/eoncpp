cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonEdtTests LANGUAGES CXX)
message(STATUS " - EonEdtTests")

set(EDTTEST_HEADERS
	Regression.h
)
set(EDTTEST_SOURCES
	Regression.cpp
)

add_executable(EonEdtTests ${EDTTEST_HEADERS} ${EDTTEST_SOURCES})
set_target_properties(
	EonEdtTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_edt_tests
		OUTPUT_NAME_RELEASE eon_edt_tests
)
set_property(TARGET EonEdtTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonEdtTests PUBLIC EonEdt EonTest EonRegex)
	
install(TARGETS EonEdtTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonEdtTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
