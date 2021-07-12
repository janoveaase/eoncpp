cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonPath LANGUAGES CXX)
message(STATUS " - EonPath")

set(PATH_HEADERS
	Path.h
)
set(PATH_SOURCES
	Path.cpp
)

add_library(EonPath STATIC ${PATH_HEADERS} ${PATH_SOURCES})
set_target_properties(
	EonPath
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_path
		OUTPUT_NAME_RELEASE eon_path
)
set_property(TARGET EonPath PROPERTY CXX_STANDARD 17)

target_link_libraries(EonPath PUBLIC EonString EonName EonExcept)
target_include_directories(EonPath PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonPath
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${PATH_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonpath")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonPath.dir/Debug/EonPath.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
