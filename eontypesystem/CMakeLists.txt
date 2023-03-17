cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTypeSystem LANGUAGES CXX)
message(STATUS " - EonTypeSystem")

set(TYPESYSTEM_HEADERS
	Definitions.h
	TypeMap.h
	BoolActions.h
	ByteActions.h
	CharActions.h
	IntActions.h
	FloatActions.h
	IndexActions.h
	NameActions.h
	HandleActions.h
	BitsActions.h
	BytesActions.h
	StringActions.h
	TupleDefs.h
	TypeTuple.h
	TypeTupleFactory.h
	Tuple.h
	TupleActions.h
	TupleFactory.h
	Attribute.h
	Action.h
	Operators.h
)
set(TYPESYSTEM_SOURCES
	TypeMap.cpp
	BoolActions.cpp
	ByteActions.cpp
	CharActions.cpp
	IndexActions.cpp
	NameActions.cpp
	HandleActions.cpp
	BytesActions.cpp
	StringActions.cpp
	TupleDefs.cpp
	TypeTuple.cpp
	TypeTupleFactory.cpp
	Tuple.cpp
	TupleFactory.cpp
	TupleActions.cpp
	Attribute.cpp
	Operators.cpp
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

set(libraries
	EonString
	EonExcept
	EonRegex
	EonSource
	EonFileSys
	EonContainers
	EonTokenizer
	EonInlineTest
	EonExpression
)

target_link_libraries(EonTypeSystem PUBLIC ${libraries})
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
if(EON_INLINE_TESTS)
	message(STATUS "Including Inline Tests!")
	add_executable(EonTypeSystemInlineTests ${TYPESYSTEM_HEADERS} ${TYPESYSTEM_SOURCES})
	target_compile_definitions(EonTypeSystemInlineTests PRIVATE EON_INLINE_TEST_PROJECT)
	set_target_properties(
		EonTypeSystemInlineTests
		PROPERTIES
			OUTPUT_NAME_DEBUG eon_typesystem_inlinetests
			OUTPUT_NAME_RELEASE eon_typesystem_inlinetests
	)
	set_property(TARGET EonTypeSystemInlineTests PROPERTY CXX_STANDARD 17)

	target_link_libraries(EonTypeSystemInlineTests PUBLIC ${libraries})

	install(TARGETS EonTypeSystemInlineTests
		RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
	)

	target_include_directories(EonTypeSystemInlineTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
endif()
