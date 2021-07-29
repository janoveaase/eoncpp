cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonExpressionTests LANGUAGES CXX)
message(STATUS " - EonExpressionTests")

set(EXPRESSIONTEST_HEADERS
	Regression.h
)
set(EXPRESSIONTEST_SOURCES
	Regression.cpp
)

add_executable(EonExpressionTests
	${EXPRESSIONTEST_HEADERS} ${EXPRESSIONTEST_SOURCES}
)
set_target_properties(
	EonExpressionTests
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_expression_tests
		OUTPUT_NAME_RELEASE eon_expression_tests
)
set_property(TARGET EonExpressionTests PROPERTY CXX_STANDARD 17)

target_link_libraries(EonExpressionTests PUBLIC EonExpression
	EonTest EonExcept)
	
install(TARGETS EonExpressionTests
	RUNTIME DESTINATION "${EON_INSTALL_DIR}/${EON_TESTS_DIR}"
)

target_include_directories(EonExpressionTests PUBLIC "${EON_SOURCES_DIR}"
	"${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")
