project(EonTypeSystem LANGUAGES CXX)

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
)
eon_add_inlinetests()
