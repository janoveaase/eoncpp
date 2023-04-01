function(eon_add_inlinetests)
	if(NOT (EON_INLINE_TESTS AND BUILD_TESTING))
		return()
	endif()

	message(STATUS " --> ${PROJECT_NAME}InlineTests")
	add_executable(${PROJECT_NAME}InlineTests)

	# We assume that ${PROJECT_NAME} only includes SOURCES but links to an INTERFACE
	# library that provides all usage requirements, otherwise we would also need to
	# extract definitions, features, includedirs, etc from ${PROJECT_NAME}.
	target_sources(${PROJECT_NAME}InlineTests PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME},SOURCES>)
	target_link_libraries(${PROJECT_NAME}InlineTests
		PRIVATE
			EonInlineTest
			$<TARGET_PROPERTY:${PROJECT_NAME},INTERFACE_LINK_LIBRARIES>
	)

	set_target_properties(${PROJECT_NAME}InlineTests PROPERTIES FOLDER "Eon/${PROJECT_NAME}")
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
