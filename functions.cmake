function(eon_add_library)
	message(STATUS " -> ${PROJECT_NAME}")

	add_library(${PROJECT_NAME})
	file(GLOB sources *.cpp *.natvis)
	list(FILTER sources INCLUDE REGEX "[A-Z][a-zA-Z]+\.cpp" )
	file(GLOB public_headers *.h)
	target_sources(${PROJECT_NAME}
		PRIVATE
			${sources}
		PUBLIC FILE_SET HEADERS
			BASE_DIRS ..
			FILES
				${public_headers}
	)
	target_include_directories(${PROJECT_NAME}
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
			$<INSTALL_INTERFACE:include>
	)
	target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
	set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Eon/${PROJECT_NAME}")

	install(
		TARGETS ${PROJECT_NAME}
		EXPORT EonCpp
		FILE_SET HEADERS
	)
endfunction()
