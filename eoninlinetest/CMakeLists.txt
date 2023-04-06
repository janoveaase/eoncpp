function(eon_add_inlinetests)
	if(NOT (EON_INLINE_TESTS AND BUILD_TESTING))
		return()
	endif()

	message(STATUS " --> ${PROJECT_NAME}InlineTests")
	add_executable(${PROJECT_NAME}InlineTests)

	# Since we're compiling source files again while adding INLINE_TEST compile
	# definitions, copy all sources and usage requirements from the original target.
	# This is not really nice, but the alternative is to demand that each upstream
	# target depends on a ${PROJECT_NAME}_config INTERFACE target which provides them.
	set_target_properties(${PROJECT_NAME}InlineTests PROPERTIES
		SOURCES $<TARGET_PROPERTY:${PROJECT_NAME},SOURCES>
		COMPILE_DEFINITIONS $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_COMPILE_DEFINITIONS>
		COMPILE_FEATURES $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_COMPILE_FEATURES>
		COMPILE_OPTIONS $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_COMPILE_OPTIONS>
		INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_INCLUDE_DIRECTORIES>
		LINK_LIBRARIES $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_LINK_LIBRARIES>
		LINK_OPTIONS $<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_LINK_OPTIONS>
		FOLDER "Eon/${PROJECT_NAME}"
	)

	target_link_libraries(${PROJECT_NAME}InlineTests PRIVATE EonInlineTest)
	add_test(NAME ${PROJECT_NAME}InlineTests COMMAND ${PROJECT_NAME}InlineTests)
	install(TARGETS ${PROJECT_NAME}InlineTests DESTINATION testbin)
endfunction()



project(EonInlineTest LANGUAGES CXX)

eon_add_library()
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		EonString
		EonTerminal
)

option(EON_TEST_LOCALE_NO "Check if Norwegian locale is present on host computer")
target_compile_definitions(${PROJECT_NAME}
	INTERFACE
		EON_INLINE_TEST_PROJECT
		EON_INLINE_TEST
		$<$<BOOL:${EON_TEST_LOCALE_NO}>:EON_TEST_LOCALE_NO>
)
