project(EonSource LANGUAGES CXX)

eon_add_library()
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		EonString
		EonExcept
)
eon_add_inlinetests()
eon_add_tests()
