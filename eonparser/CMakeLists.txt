project(EonParser LANGUAGES CXX)

eon_add_library()
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		EonString
		EonExcept
		EonRegex
		EonSource
		EonFileSys
		EonContainers
		EonTokenizer
		EonInlineTest
		EonExpression
		EonTypeSystem
)
eon_add_inlinetests()
eon_add_tests()
