cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTypes LANGUAGES CXX)
message(STATUS " - EonTypes")

set(TYPES_HEADERS
	TypeDefinitions.h
	TypeSystem.h
	TypeHandler.h
	Attribute.h
	Caches.h
	BasicTuple.h
	BasicTupleActions.h
	DataTuple.h
	DynamicTuple.h
	TypeTuple.h
	Tuple.h
	Bool.h
	BoolActions.h
	Byte.h
	ByteActions.h
	Char.h
	CharActions.h
	Integer.h
	IntegerActions.h
	Floatingpt.h
	FloatingptActions.h
	Index.h
	IndexActions.h
	Name.h
	NameActions.h
	Handle.h
	HandleActions.h
	Shared.h
	Reference.h
	Expression.h
	ExpressionObj.h
	ExpressionActions.h
	String.h
	StringActions.h
	Bytes.h
	BytesActions.h
	Path.h
	PathActions.h
	Regex.h
	RegexActions.h
	NamePath.h
	NamePathActions.h
	Action.h
	Operators.h
	OperatorAction.h
	Node.h
	Stringifier.h
	Primitives.h
	StandardTypes.h
)
set(TYPES_SOURCES
	TypeSystem.cpp
	TypeHandler.cpp
	Attribute.cpp
	BasicTuple.cpp
	BasicTupleActions.cpp
	TypeTuple.cpp
	Bool.cpp
	BoolActions.cpp
	Byte.cpp
	ByteActions.cpp
	Char.cpp
	CharActions.cpp
	Index.cpp
	IndexActions.cpp
	Name.cpp
	NameActions.cpp
	Handle.cpp
	HandleActions.cpp
	Shared.cpp
	Expression.cpp
	ExpressionObj.cpp
	ExpressionActions.cpp
	String.cpp
	StringActions.cpp
	Bytes.cpp
	BytesActions.cpp
	Path.cpp
	PathActions.cpp
	Regex.cpp
	RegexActions.cpp
	NamePath.cpp
	NamePathActions.cpp
	Action.cpp
	Operators.cpp
	OperatorAction.cpp
	Node.cpp
	Primitives.cpp
	StandardTypes.cpp
)

add_library(EonTypes STATIC ${TYPES_HEADERS} ${TYPES_SOURCES})
set_target_properties(
	EonTypes
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_types
		OUTPUT_NAME_RELEASE eon_types
)
set_property(TARGET EonTypes PROPERTY CXX_STANDARD 17)

target_link_libraries(EonTypes PUBLIC EonString EonExcept EonHex EonRegex EonScopes EonFileSys)
target_include_directories(EonTypes PUBLIC "${EON_SOURCES_DIR}"
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}"
)

install(TARGETS EonTypes
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${TYPES_HEADERS} DESTINATION
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eontypes"
)
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonTypes.dir/Debug/EonTypes.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL
	)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
