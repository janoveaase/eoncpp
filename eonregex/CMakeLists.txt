cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonRegex LANGUAGES CXX)
message(STATUS " - EonRegex")

set(REGEX_HEADERS
	RegEx.h
	RxDefs.h
	Graph.h
	Node.h
	NodeGroup.h
	Quantifier.h
	RxData.h
	CharGroup.h
	OpOr.h
	sources/LocStart.h
	sources/LocEnd.h
	sources/OpNot.h
	sources/FixedValue.h
	sources/Backreference.h
	sources/LocWordBoundary.h
	sources/WordChar.h
	sources/Special.h
	sources/Case.h
	sources/Digit.h
	sources/Any.h
	sources/CaptureGroup.h
	Match.h
)
set(REGEX_SOURCES
	RegEx.cpp
	sources/Graph.cpp
	sources/Node.cpp
	sources/NodeGroup.cpp
	sources/RxData.cpp
	sources/CharGroup.cpp
	sources/OpOr.cpp
	sources/FixedValue.cpp
	sources/Backreference.cpp
	sources/LocWordBoundary.cpp
	sources/CaptureGroup.cpp
)

add_library(EonRegex STATIC ${REGEX_HEADERS} ${REGEX_SOURCES})
set_target_properties(
	EonRegex
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_regex
		OUTPUT_NAME_RELEASE eon_regex
)
set_property(TARGET EonRegex PROPERTY CXX_STANDARD 17)

target_link_libraries(EonRegex PUBLIC EonString EonExcept)
target_include_directories(EonRegex PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonRegex
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${REGEX_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonregex")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonRegex.dir/Debug/EonRegex.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
