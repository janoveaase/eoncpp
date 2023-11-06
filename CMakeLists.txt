cmake_minimum_required(VERSION 3.25)
project( Eon
	VERSION 0.1.1
)

message(STATUS "${PROJECT_NAME} C++ v${PROJECT_VERSION}")
include(GNUInstallDirs)
include(CTest)

include(configuration.cmake)
include(functions.cmake)
include(modules.cmake)

install(FILES eoncppConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EonCpp )
