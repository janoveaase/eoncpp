project(EonRegex LANGUAGES CXX)

eon_add_library()
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		EonString
		EonExcept
		EonContainers
)
#eon_add_inlinetests()
