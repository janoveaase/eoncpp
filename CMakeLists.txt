cmake_minimum_required(VERSION 3.25)
project( Eon
	VERSION 0.1.1
)

message(STATUS "${PROJECT_NAME} C++ v${PROJECT_VERSION}")
include(GNUInstallDirs)
include(CTest)

# Configuration
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
option(EON_INLINE_TESTS "Include inline unit tests in build?" ON)
option(EON_BUILD_FILESYS "Include the File System package" ON)
option(EON_BUILD_TYPESYSTEM "Include the Type System package" ON)
#option(EON_BUILD_EDT "Build the Eon Data Tuple package")
option(EON_BUILD_EDOC "Build the Eon Documentation package")
option(EON_BUILD_TOKENIZER "Build the Tokenizer package")
option(EON_BUILD_PARSER "Build the Eon Parser package" ON)
option(EON_BUILD_EONEDOCGEN "Build the Eon Document Generator")
option(EON_STATIC_RUNTIME "Use static runtime libraries" ON)

if(EON_STATIC_RUNTIME)
	message(STATUS "Using static runtime.")
	if(CMAKE_C_COMPILER_ID STREQUAL GNU)
		add_link_options(-static-libgcc)
	endif()
	if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
		add_link_options(-static-libgcc -static-libstdc++)
	endif()
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

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

add_subdirectory(eontest)
add_subdirectory(eoninlinetest)

add_subdirectory(eonterminal)
add_subdirectory(eoncontainers)
#add_subdirectory(eonstack)
#add_subdirectory(eonreflexivemap)
add_subdirectory(eonstring)
add_subdirectory(eonexcept)
add_subdirectory(eonregex)
add_subdirectory(eonsource)
if(EON_BUILD_TYPESYSTEM OR EON_BUILD_PARSER)
	add_subdirectory(eontypesystem)
	add_subdirectory(eonexpression)
endif()
if(EON_BUILD_FILESYS)
	add_subdirectory(eonfilesys)
endif()
#if(EON_BUILD_TOKENIZER OR EON_BUILD_PARSER OR EON_BUILD_EDT OR EON_BUILD_EONEDOCGEN)
if(EON_BUILD_TOKENIZER OR EON_BUILD_TYPESYSTEM OR EON_BUILD_PARSER)
	add_subdirectory(eontokenizer)
endif()
#if(EON_BUILD_TOKENIZER OR EON_BUILD_PARSER OR EON_BUILD_EDT OR EON_BUILD_EDOC)
#	add_subdirectory(eontypes)
#endif()
if(EON_BUILD_EDOC OR EON_BUILD_EONEDOCGEN)
	add_subdirectory(eonedoc)
endif()
if(EON_BUILD_EONEDOCGEN)
	add_subdirectory(eonedocgen)
endif()
#if(EON_BUILD_EDT OR EON_BUILD_EONEDOCGEN)
#	add_subdirectory(eonedt)
#endif()
if(EON_BUILD_PARSER)
	add_subdirectory(eonparser)
endif()
