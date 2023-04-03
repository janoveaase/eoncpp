project(EonRegex LANGUAGES CXX)

eon_add_library()
file(GLOB private_sources sources/*.h sources/*.cpp sources/*.natvis )
target_sources(${PROJECT_NAME} PRIVATE ${private_sources})
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		EonString
		EonExcept
		EonContainers
)
#eon_add_inlinetests()
eon_add_tests()
