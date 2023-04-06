project(EonEdt LANGUAGES CXX)

eon_add_library()
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		EonString
		EonParser
		EonSource
		EonTokenizer
		EonRegex
		EonExcept
		EonFileSys
		EonTypes
)
#eon_add_inlinetests()
eon_add_tests()
