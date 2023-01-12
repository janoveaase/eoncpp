cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTypeSystemTests LANGUAGES CXX)
message(STATUS " - EonTypeSystemTests")

set(TYPESYSTEMTEST_HEADERS
	Regression.h
)
set(TYPESYSTEMTEST_SOURCES
	Regression.cpp
)

add_executable(EonTypeSystemTests ${TYPESYSTEMTEST_HEADERS} ${TYPESYSTEMTEST_SOURCES})
set_target_properties(
	EonTypeSystemTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_typesystem_tests
		OUTPUT_NAME_RELEASE eon_typesystem_tests
)
set_property(TARGET EonTypeSystemTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonTypeSystemTests PUBLIC EonTypeSystem EonTest)
	
install(TARGETS EonTypeSystemTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonTypeSystemTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
