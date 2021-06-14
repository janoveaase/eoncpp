cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTF LANGUAGES CXX)
message(STATUS " - EonTF")

set(ETF_HEADERS
	Etf.h
	Parser.h
)
set(ETF_SOURCES
	Etf.cpp
	Parser.cpp
)

add_library(EonTF STATIC ${ETF_HEADERS} ${ETF_SOURCES})
set_target_properties(
	EonTF
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_tf
		OUTPUT_NAME_RELEASE eon_tf
)

target_link_libraries(EonTF PUBLIC EonString EonName EonExcept EonRegex EonTuple EonPath EonFile)
target_include_directories(EonTF PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonTF
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${ETF_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eontf")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonTF.dir/Debug/EonTF.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
