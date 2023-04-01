project(EonExpression LANGUAGES CXX)

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
		EonTypeSystem
)
eon_add_inlinetests()
