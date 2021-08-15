cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonContainers LANGUAGES CXX)
message(STATUS " - EonContainers")

set(CONTAIERS_HEADERS
	Vector.h
	Stack.h
)

add_library(EonContainers INTERFACE)

target_include_directories(EonContainers INTERFACE "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

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
