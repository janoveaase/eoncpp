project(EonDoc LANGUAGES CXX)

eon_add_library()
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		EonString
		EonName
		EonExcept
		EonPath
		EonFile
		EonTokenizer
)
#eon_add_inlinetests()
