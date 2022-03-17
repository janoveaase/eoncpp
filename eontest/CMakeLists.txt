cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonTest LANGUAGES CXX)
message(STATUS " - EonTest")

set(TEST_HEADERS
	Test.h
)
set(TEST_SOURCES
	Test.cpp
	Main.cpp
)

add_library(EonTest STATIC ${TEST_HEADERS} ${TEST_SOURCES})
set_target_properties(
	EonTest
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_test
		OUTPUT_NAME_RELEASE eon_test
)
set_property(TARGET EonTest PROPERTY CXX_STANDARD 17)

target_link_libraries(EonTest PUBLIC EonString EonTerminal)
install(TARGETS EonTest
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${TEST_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eontest")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonTest.dir/Debug/EonTest.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()
