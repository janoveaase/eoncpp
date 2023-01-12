cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonExcept LANGUAGES CXX)
message(STATUS " - EonExcept")

set(EXCEPT_HEADERS
	Exception.h
)
set(EXCEPT_SOURCES
	Exception.cpp
)

add_library(EonExcept STATIC ${EXCEPT_HEADERS} ${EXCEPT_SOURCES})
set_target_properties(
	EonExcept
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_except
		OUTPUT_NAME_RELEASE eon_except
)
set_property(TARGET EonExcept PROPERTY CXX_STANDARD 17)

target_link_libraries(EonExcept PUBLIC EonString)
target_include_directories(EonExcept INTERFACE "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonExcept
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${EXCEPT_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonexcept")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonExcept.dir/Debug/EonExcept.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
if(EON_INLINE_TESTS)
	message(STATUS "Including Inline Tests!")
	add_executable(EonExceptInlineTests ${EXCEPT_HEADERS} ${EXCEPT_SOURCES})
	target_compile_definitions(EonExceptInlineTests PRIVATE EON_INLINE_TEST_PROJECT)
	set_target_properties(
		EonExceptInlineTests
		PROPERTIES
			OUTPUT_NAME_DEBUG eon_except_inlinetests
			OUTPUT_NAME_RELEASE eon_except_inlinetests
	)
	set_property(TARGET EonExceptInlineTests PROPERTY CXX_STANDARD 17)

	target_link_libraries(EonExceptInlineTests PUBLIC EonInlineTest)

	install(TARGETS EonExceptInlineTests
		RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
	)

	target_include_directories(EonExceptInlineTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
endif()
