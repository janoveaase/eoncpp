cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonContainers LANGUAGES CXX)
message(STATUS " - EonContainers")

set(CONTAINERS_HEADERS
	Storage.h
	Vector.h
	Stack.h
	ReflexiveMap.h
)
set(CONTAINERS_SOURCES
	Storage.cpp
	Vector.cpp
)

add_library(EonContainers STATIC ${CONTAINERS_HEADERS} ${CONTAINERS_SOURCES})
set_target_properties(
	EonContainers
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_containers
		OUTPUT_NAME_RELEASE eon_containers
)
set_property(TARGET EonContainers PROPERTY CXX_STANDARD 17)

target_link_libraries(EonContainers PUBLIC EonString EonExcept)
target_include_directories(EonContainers PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonContainers
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${CONTAINERS_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eoncontainers")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonContainers.dir/Debug/EonContainers.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
if(EON_INLINE_TESTS)
	message(STATUS "Including Inline Tests!")
	add_executable(EonContainersInlineTests ${CONTAINERS_HEADERS} ${CONTAINERS_SOURCES})
	target_compile_definitions(EonContainersInlineTests PRIVATE EON_INLINE_TEST_PROJECT)
	set_target_properties(
		EonContainersInlineTests
		PROPERTIES
			OUTPUT_NAME_DEBUG eon_containers_inlinetests
			OUTPUT_NAME_RELEASE eon_containers_inlinetests
	)
	set_property(TARGET EonContainersInlineTests PROPERTY CXX_STANDARD 17)

	target_link_libraries(EonContainersInlineTests PUBLIC EonInlineTest)

	install(TARGETS EonContainersInlineTests
		RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
	)

	target_include_directories(EonContainersInlineTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
endif()
