cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonFile LANGUAGES CXX)
message(STATUS " - EonFile")

set(FILE_HEADERS
	File.h
	FileDefs.h
	FileBuffer.h
	TextFileReader.h
	TextFileWriter.h
	BinaryFileReader.h
	BinaryFileWriter.h
)
set(FILE_SOURCES
	FileBuffer.cpp
	TextFileReader.cpp
	TextFileWriter.cpp
	BinaryFileReader.cpp
)

add_library(EonFile STATIC ${FILE_HEADERS} ${FILE_SOURCES})
set_target_properties(
	EonFile
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_file
		OUTPUT_NAME_RELEASE eon_file
)

target_link_libraries(EonFile PUBLIC EonString EonName EonExcept EonPath)
target_include_directories(EonFile PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonFile
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${FILE_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonfile")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonFile.dir/Debug/EonFile.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
