cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonName LANGUAGES CXX)
message(STATUS " - EonName")

set(NAME_HEADERS
	Name.h
)
set(NAME_SOURCES
	Name.cpp
)

if(WIN32)
	set(NAME_OTHER name.natvis)
endif()

add_library(EonName STATIC ${NAME_HEADERS} ${NAME_SOURCES} ${NAME_OTHER})
set_target_properties(
	EonName
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_name
		OUTPUT_NAME_RELEASE eon_name
)
set_property(TARGET EonName PROPERTY CXX_STANDARD 17)

target_link_libraries(EonName PUBLIC EonString)
target_include_directories(EonName PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonName
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${NAME_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonname")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonName.dir/Debug/EonName.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
