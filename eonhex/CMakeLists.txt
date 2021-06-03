cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonHex LANGUAGES CXX)
message(STATUS " - EonHex")

set(HEX_HEADERS
	Hex.h
)
set(HEX_SOURCES
	Hex.cpp
)

add_library(EonHex STATIC ${HEX_HEADERS} ${HEX_SOURCES})
set_target_properties(
	EonHex
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_hex
		OUTPUT_NAME_RELEASE eon_hex
)

target_link_libraries(EonHex PUBLIC EonString EonName EonExcept)
target_include_directories(EonHex PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonHex
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${HEX_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonhex")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonHex.dir/Debug/EonHex.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
