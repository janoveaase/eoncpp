cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonDoc LANGUAGES CXX)
message(STATUS " - EonDoc")

set(EDOC_HEADERS
	Edoc.h
	Document.h
	Element.h
	Block.h
	Title.h
	Heading.h
	Paragraph.h
	Section.h
	Listing.h
	Definition.h
	Insert.h
	Exclamation.h
	Table.h
	Parser.h
)
set(EDOC_SOURCES
	Edoc.cpp
	Paragraph.cpp
	Listing.cpp
	Definition.cpp
	Insert.cpp
	Exclamation.cpp
	Table.cpp
	Parser.cpp
)

add_library(EonDoc STATIC ${EDOC_HEADERS} ${EDOC_SOURCES})
set_target_properties(
	EonDoc
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_doc
		OUTPUT_NAME_RELEASE eon_doc
)
set_property(TARGET EonDoc PROPERTY CXX_STANDARD 17)

target_link_libraries(EonDoc PUBLIC EonString EonName EonExcept EonPath EonFile EonTokenizer)
target_include_directories(EonDoc PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonDoc
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${EDOC_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eondoc")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonDoc.dir/Debug/EonDoc.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
