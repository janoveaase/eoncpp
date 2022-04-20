cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonEdt LANGUAGES CXX)
message(STATUS " - EonEdt")

set(EDT_HEADERS
	Edt.h
)
set(EDT_SOURCES
	Edt.cpp
)

add_library(EonEdt STATIC ${EDT_HEADERS} ${EDT_SOURCES})
set_target_properties(
	EonEdt
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_edt
		OUTPUT_NAME_RELEASE eon_edt
)
set_property(TARGET EonEdt PROPERTY CXX_STANDARD 17)

target_link_libraries(EonEdt PUBLIC EonString
	EonSource EonTokenizer EonRegex EonExcept EonFileSys EonTypes EonParser
)
target_include_directories(EonEdt PUBLIC "${EON_SOURCES_DIR}"
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}"
)

install(TARGETS EonEdt
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${EDT_HEADERS} DESTINATION
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonedt"
)
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/eon_edt_d.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
