cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonEdocGen LANGUAGES CXX)
message(STATUS " - EonEdocGen")

set(EDOCGEN_HEADERS
	Generator.h
)
set(EDOCGEN_SOURCES
	Generator.cpp
	Main.cpp
)

if(WIN32)
	set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_executable(EonEdocGen ${EDOCGEN_HEADERS} ${EDOCGEN_SOURCES})
set_target_properties(
	EonEdocGen
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_edocgen
		OUTPUT_NAME_RELEASE eon_edocgen
)
set_property(TARGET EonEdocGen PROPERTY CXX_STANDARD 17)

target_link_libraries(EonEdocGen PUBLIC EonEdoc EonTerminal EonFileSys EonEdt)
target_include_directories(EonEdocGen PUBLIC "${EON_SOURCES_DIR}"
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}"
)

install(TARGETS EonEdocGen
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_BINARY_DIR}"
)
