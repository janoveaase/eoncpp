cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonScopesTests LANGUAGES CXX)
message(STATUS " - EonScopesTests")

set(SCOPESTEST_HEADERS
	Regression.h
)
set(SCOPESTEST_SOURCES
	Regression.cpp
)

add_executable(EonScopesTests ${SCOPESTEST_HEADERS} ${SCOPESTEST_SOURCES})
set_target_properties(
	EonScopesTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_scopes_tests
		OUTPUT_NAME_RELEASE eon_scopes_tests
)
set_property(TARGET EonScopesTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonScopesTests PUBLIC EonScopes EonTest)
	
install(TARGETS EonScopesTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonScopesTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
