cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonExpression LANGUAGES CXX)
message(STATUS " - EonExpression")

set(EXPRESSION_HEADERS
	Expression.h
	ExpressionNode.h
)
set(EXPRESSION_SOURCES
	Expression.cpp
	ExpressionNode.cpp
)

if(WIN32)
	set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(EonExpression STATIC ${EXPRESSION_HEADERS} ${EXPRESSION_SOURCES})
set_target_properties(
	EonExpression
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_expression
		OUTPUT_NAME_RELEASE eon_expression
)
set_property(TARGET EonExpression PROPERTY CXX_STANDARD 17)

set(libraries
	EonString
	EonExcept
	EonRegex
	EonSource
	EonFileSys
	EonContainers
	EonTokenizer
	EonInlineTest
	EonTypeSystem
)

target_link_libraries(EonExpression PUBLIC ${libraries})
target_include_directories(EonExpression PUBLIC "${EON_SOURCES_DIR}"
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}"
)

install(TARGETS EonExpression
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${EXPRESSION_HEADERS} DESTINATION
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonexpression"
)
if(WIN32)
	install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}/Debug/eon_expression${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_INLINE_TESTS)
	message(STATUS "Including Inline Tests!")
	add_executable(EonExpressionInlineTests ${EXPRESSION_HEADERS} ${EXPRESSION_SOURCES})
	target_compile_definitions(EonExpressionInlineTests PRIVATE EON_INLINE_TEST_PROJECT)
	set_target_properties(
		EonExpressionInlineTests
		PROPERTIES
			OUTPUT_NAME_DEBUG eon_expression_inlinetests
			OUTPUT_NAME_RELEASE eon_expression_inlinetests
	)
	set_property(TARGET EonExpressionInlineTests PROPERTY CXX_STANDARD 17)

	target_link_libraries(EonExpressionInlineTests PUBLIC ${libraries})

	install(TARGETS EonExpressionInlineTests
		RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
	)

	target_include_directories(EonExpressionInlineTests PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
endif()
