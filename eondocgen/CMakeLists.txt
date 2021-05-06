cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonDocGen LANGUAGES CXX)
message(STATUS " - EonDocGen")

set(DOCGEN_HEADERS
	DocGen.h
	DocNode.h
)
set(DOCGEN_SOURCES
	DocGen.cpp
	DocNode.cpp
)

add_library(EonDocGen STATIC ${DOCGEN_HEADERS} ${DOCGEN_SOURCES})
set_target_properties(
	EonDocGen
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_docgen
		OUTPUT_NAME_RELEASE eon_docgen
)

target_link_libraries(EonDocGen PUBLIC EonString EonSource EonTokenizer EonRegex EonExcept)
target_include_directories(EonDocGen PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonDocGen
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${DOCGEN_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eondocgen")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonDocGen.dir/Debug/EonDocGen.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
