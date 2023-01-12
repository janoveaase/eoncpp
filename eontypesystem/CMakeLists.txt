cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTypeSystem LANGUAGES CXX)
message(STATUS " - EonTypeSystem")

set(TYPESYSTEM_HEADERS
	Definitions.h
	TypeMap.h
	Bool.h
	Byte.h
	Char.h
	IntTypes.h
	FloatTypes.h
	Index.h
	Name.h
	Handle.h
	BitsTypes.h
	BytesActions.h
	StringActions.h
	Expression.h
	ExpressionNode.h
	TupleDefs.h
	TypeTuple.h
	Tuple.h
	TupleActions.h
	TupleFactory.h
	Attribute.h
	Action.h
	Operators.h
	DirectConversion.h
)
set(TYPESYSTEM_SOURCES
	TypeMap.cpp
	Bool.cpp
	Byte.cpp
	Char.cpp
	Index.cpp
	Name.cpp
	Handle.cpp
	BytesActions.cpp
	StringActions.cpp
	Expression.cpp
	ExpressionNode.cpp
	TupleDefs.cpp
	TypeTuple.cpp
	Tuple.cpp
	TupleFactory.cpp
	TupleActions.cpp
	Attribute.cpp
	Operators.cpp
	DirectConversion.cpp
)

if(WIN32)
	set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(EonTypeSystem STATIC ${TYPESYSTEM_HEADERS} ${TYPESYSTEM_SOURCES})
set_target_properties(
	EonTypeSystem
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_typesystem
		OUTPUT_NAME_RELEASE eon_typesystem
)
set_property(TARGET EonTypeSystem PROPERTY CXX_STANDARD 17)

target_link_libraries(EonTypeSystem PUBLIC EonString EonExcept EonRegex EonSource EonFileSys EonContainers EonTokenizer)
target_include_directories(EonTypeSystem PUBLIC "${EON_SOURCES_DIR}"
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}"
)

install(TARGETS EonTypeSystem
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${TYPESYSTEM_HEADERS} DESTINATION
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eontypesystem"
)
if(WIN32)
	install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}/Debug/eon_typesystem${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
