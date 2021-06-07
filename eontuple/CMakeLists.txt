cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTuple LANGUAGES CXX)
message(STATUS " - EonTuple")

set(TUPLE_HEADERS
	Value.h
	BoolValue.h
	CharValue.h
	IntValue.h
	FloatValue.h
	NameValue.h
	StringValue.h
	BinaryValue.h
	RawValue.h
	RegexValue.h
	ReferenceValue.h
	MetaValue.h
	TupleValue.h
	Path.h
	Tuple.h
)
set(TUPLE_SOURCES
	Value.cpp
	BinaryValue.cpp
	RawValue.cpp
	TupleValue.cpp
	Path.cpp
	Tuple.cpp
)

add_library(EonTuple STATIC ${TUPLE_HEADERS} ${TUPLE_SOURCES})
set_target_properties(
	EonTuple
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_tuple
		OUTPUT_NAME_RELEASE eon_tuple
)

target_link_libraries(EonTuple PUBLIC EonString EonName EonExcept EonHex EonRegex)
target_include_directories(EonTuple PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonTuple
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${TUPLE_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eontuple")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonTuple.dir/Debug/EonTuple.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
