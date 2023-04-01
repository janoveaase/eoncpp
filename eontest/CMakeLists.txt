function(eon_add_tests)
	if(NOT BUILD_TESTING)
		return()
	endif()

	file(GLOB cpp tests/*.cpp)
	file(GLOB hpp tests/*.h)
	if(cpp)
		message(STATUS " --> ${PROJECT_NAME}Tests")
		add_executable(${PROJECT_NAME}Tests)
		target_sources(${PROJECT_NAME}Tests PRIVATE ${cpp} ${hpp} )
		target_link_libraries(${PROJECT_NAME}Tests PRIVATE ${PROJECT_NAME} EonTest)
		set_target_properties(${PROJECT_NAME}Tests PROPERTIES FOLDER "Eon/${PROJECT_NAME}")
		add_test(NAME ${PROJECT_NAME}Tests COMMAND ${PROJECT_NAME}Tests)
		install(TARGETS ${PROJECT_NAME}Tests DESTINATION testbin)
	endif()
endfunction()


project(EonTest LANGUAGES CXX)

eon_add_library()
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		EonString
		EonTerminal
		EonRegex
)
#eon_add_inlinetests()
