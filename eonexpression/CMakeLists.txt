cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonExpression LANGUAGES CXX)
message(STATUS " - EonExpression")

set(EXPRESSION_HEADERS
	Node.h
	OperatorNode.h
	OperandNode.h
	Expression.h
	Evaluate.h
)
set(EXPRESSION_SOURCES
	OperatorNode.cpp
	OperandNode.cpp
	Expression.cpp
	Evaluate.cpp
)

add_library(EonExpression STATIC ${EXPRESSION_HEADERS} ${EXPRESSION_SOURCES})
set_target_properties(
	EonExpression
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_expression
		OUTPUT_NAME_RELEASE eon_expression
)
set_property(TARGET EonExpression PROPERTY CXX_STANDARD 17)

target_link_libraries(EonExpression PUBLIC EonString EonExcept EonTokenizer EonOF EonTuple EonFile)
target_include_directories(EonExpression PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonExpression
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${EXPRESSION_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonexpression")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonExpression.dir/Debug/EonExpression.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
