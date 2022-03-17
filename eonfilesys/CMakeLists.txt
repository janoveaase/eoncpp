cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonFileSys LANGUAGES CXX)
message(STATUS " - EonFileSys")

set(FILESYS_HEADERS
	FileSys.h
	Path.h
	File.h
	Dir.h
	FileBuffer.h
)
set(FILESYS_SOURCES
	Path.cpp
	File.cpp
	Dir.cpp
	FileBuffer.cpp
)

add_library(EonFileSys STATIC ${FILESYS_HEADERS} ${FILESYS_SOURCES})
set_target_properties(
	EonFileSys
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_filesys
		OUTPUT_NAME_RELEASE eon_filesys
)
set_property(TARGET EonFileSys PROPERTY CXX_STANDARD 17)

target_link_libraries(EonFileSys PUBLIC EonString EonExcept)
target_include_directories(EonFileSys PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonFileSys
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${FILESYS_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonfilesys")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonFileSys.dir/Debug/EonFileSys.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
