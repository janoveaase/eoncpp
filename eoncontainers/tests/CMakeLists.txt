cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonContainersTests LANGUAGES CXX)
message(STATUS " - EonContainersTests")

set(CONTAINERSTEST_HEADERS
	Regression.h
)
set(CONTAINERSTEST_SOURCES
	Regression.cpp
)

add_executable(EonContainersTests ${CONTAINERSTEST_HEADERS} ${CONTAINERSTEST_SOURCES})
set_target_properties(
	EonContainersTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_containers_tests
		OUTPUT_NAME_RELEASE eon_containers_tests
)
set_property(TARGET EonContainersTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonContainersTests PUBLIC EonContainers EonRegex EonTest)
	
install(TARGETS EonContainersTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonContainersTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
