cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonVariables LANGUAGES CXX)
message(STATUS " - EonVariables")

set(VARIABLES_HEADERS
	Value.h
	BoolValue.h
	CharValue.h
	IntValue.h
	FloatValue.h
	NameValue.h
	StringValue.h
	RawValue.h
	BinaryValue.h
	RegexValue.h
	ReferenceValue.h
	VariableValue.h
	ExpressionValue.h
	TupleValue.h
	MetaValue.h
	FunctionValue.h
	Function.h
	ControlValue.h
	Operators.h
	Variables.h
)
set(VARIABLES_SOURCES
	Value.cpp
	RawValue.cpp
	BinaryValue.cpp
	ReferenceValue.cpp
	VariableValue.cpp
	TupleValue.cpp
	FunctionValue.cpp
	Operators.cpp
	Variables.cpp
)

add_library(EonVariables STATIC ${VARIABLES_HEADERS} ${VARIABLES_SOURCES})
set_target_properties(
	EonVariables
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_variables
		OUTPUT_NAME_RELEASE eon_variables
)
set_property(TARGET EonVariables PROPERTY CXX_STANDARD 17)

target_link_libraries(EonVariables PUBLIC EonString
	EonName EonExcept EonContainers EonHex EonRegex EonExpression EonFileSys
)
target_include_directories(EonVariables PUBLIC "${EON_SOURCES_DIR}"
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}"
)

install(TARGETS EonVariables
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${VARIABLES_HEADERS} DESTINATION
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonvariables"
)
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonVariables.dir/Debug/EonVariables.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL
	)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
