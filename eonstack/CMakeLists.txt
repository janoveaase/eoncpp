cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonStack LANGUAGES CXX)
message(STATUS " - EonStack")

set(STACK_HEADERS
	Stack.h
)

add_library(EonStack INTERFACE)

target_include_directories(EonStack INTERFACE "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonStack
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${STACK_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonstack")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonStack.dir/Debug/EonStack.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
