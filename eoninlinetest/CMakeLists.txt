cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonInlineTest LANGUAGES CXX)
message(STATUS " - EonInlineTest")

set(INLINETEST_HEADERS
	InlineTest.h
	Macros.h
	Value.h
	TestFactory.h
	TestBase.h
	Timer.h
	Result.h
	TestName.h
	Location.h
	TestOptions.h
)
set(INLINETEST_SOURCES
	Main.cpp
	TestBase.cpp
	Timer.cpp
	TestOptions.cpp
)

if(WIN32)
	set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(EonInlineTest STATIC ${INLINETEST_HEADERS} ${INLINETEST_SOURCES})
set_target_properties(
	EonInlineTest
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_inlinetest
		OUTPUT_NAME_RELEASE eon_inlinetest
)
set_property(TARGET EonInlineTest PROPERTY CXX_STANDARD 17)

target_link_libraries(EonInlineTest PUBLIC EonString EonTerminal)
install(TARGETS EonInlineTest
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${TEST_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eoninlinetest")
if(WIN32)
	install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}/Debug/eon_inlinetest${CMAKE_DEBUG_POSTFIX}.pdb
		DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()
