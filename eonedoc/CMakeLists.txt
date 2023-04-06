project(EonEdoc LANGUAGES CXX)

eon_add_library()
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		EonString
		EonRegex
		EonExcept
		EonFileSys
		EonTypeSystem
)
#eon_add_inlinetests()
eon_add_tests()
