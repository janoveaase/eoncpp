cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonScopes LANGUAGES CXX)
message(STATUS " - EonScopes")

set(SCOPES_HEADERS
	Scope.h
)
set(SCOPES_SOURCES
	Scope.cpp
)

add_library(EonScopes STATIC ${SCOPES_HEADERS} ${SCOPES_SOURCES})
set_target_properties(
	EonScopes
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_scopes
		OUTPUT_NAME_RELEASE eon_scopes
)
set_property(TARGET EonScopes PROPERTY CXX_STANDARD 17)

target_link_libraries(EonScopes PUBLIC EonTypes)
target_include_directories(EonScopes PUBLIC "${EON_SOURCES_DIR}"
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}"
)

install(TARGETS EonScopes
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${scopes_HEADERS} DESTINATION
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonscopes"
)
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonScopes.dir/Debug/EonScopes.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL
	)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
