cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonEdoc LANGUAGES CXX)
message(STATUS " - EonEdoc")

set(EDOC_HEADERS
	Edoc.h
	ToHtml.h
)
set(EDOC_SOURCES
	Edoc.cpp
	ToHtml.cpp
)

if(WIN32)
	set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(EonEdoc STATIC ${EDOC_HEADERS} ${EDOC_SOURCES})
set_target_properties(
	EonEdoc
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_edoc
		OUTPUT_NAME_RELEASE eon_edoc
)
set_property(TARGET EonEdoc PROPERTY CXX_STANDARD 17)

target_link_libraries(EonEdoc PUBLIC EonString EonRegex EonExcept EonFileSys
	EonTypes
)
target_include_directories(EonEdoc PUBLIC "${EON_SOURCES_DIR}"
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}"
)

install(TARGETS EonEdoc
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${EDOC_HEADERS} DESTINATION
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonedoc"
)
if(WIN32)
	install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}/Debug/eon_edoc${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
