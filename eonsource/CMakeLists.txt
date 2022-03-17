cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonSource LANGUAGES CXX)
message(STATUS " - EonSource")

set(SOURCE_HEADERS
	Raw.h
	File.h
	String.h
	SourceRef.h
	SourcePos.h
	SourceReporter.h
)
set(SOURCE_SOURCES
	File.cpp
	String.cpp
	SourceRef.cpp
	SourceReporter.cpp
)

add_library(EonSource STATIC ${SOURCE_HEADERS} ${SOURCE_SOURCES})
set_target_properties(
	EonSource
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_source
		OUTPUT_NAME_RELEASE eon_source
)
set_property(TARGET EonSource PROPERTY CXX_STANDARD 17)

target_link_libraries(EonSource PUBLIC EonString EonExcept)
target_include_directories(EonSource PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonSource
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${SOURCE_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonsource")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonSource.dir/Debug/EonSource.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
