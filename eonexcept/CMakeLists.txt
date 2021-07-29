cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonExcept LANGUAGES CXX)
message(STATUS " - EonExcept")

set(EXCEPT_HEADERS
	Exception.h
)

add_library(EonExcept INTERFACE)
set_property(TARGET EonExcept PROPERTY CXX_STANDARD 17)

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
