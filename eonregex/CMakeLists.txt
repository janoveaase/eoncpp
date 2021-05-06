cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(EonRegEx LANGUAGES CXX)
message(STATUS " - EonRegEx")

set(REGEX_HEADERS
	RegEx.h
	sources/RxDefs.h
	sources/Graph.h
	sources/Node.h
	sources/NodeGroup.h
	sources/Quantifier.h
	sources/RxData.h
	sources/CharGroup.h
	sources/OpOr.h
	sources/LocStart.h
	sources/LocEnd.h
	sources/OpNot.h
	sources/FixedValue.h
	sources/Backreference.h
	sources/LocWordboundary.h
	sources/WordChar.h
	sources/Special.h
	sources/Case.h
	sources/Digit.h
	sources/Any.h
	sources/CaptureGroup.h
	sources/Match.h
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
	sources/LocWordboundary.cpp
	sources/CaptureGroup.cpp
)

add_library(EonRegEx STATIC ${REGEX_HEADERS} ${REGEX_SOURCES})
set_target_properties(
	EonRegEx
	PROPERTIES
		OUTPUT_NAME_DEBUG eon_regex
		OUTPUT_NAME_RELEASE eon_regex
)

target_link_libraries(EonRegEx PUBLIC EonString EonName EonExcept EonSource EonTokenizer)
target_include_directories(EonRegEx PUBLIC "${EON_SOURCES_DIR}" "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}")

install(TARGETS EonRegEx
	LIBRARY DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
	ARCHIVE DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}"
)
install(FILES ${REGEX_HEADERS} DESTINATION "${EON_INSTALL_DIR}/${EON_INCLUDE_DIR}/eonregex")
if(WIN32)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EonRegEx.dir/Debug/EonRegEx.pdb DESTINATION "${EON_INSTALL_DIR}/${EON_LIBRARY_DIR}" OPTIONAL)
endif()

if(EON_BUILD_TESTS)
	add_subdirectory(tests)
endif()
