cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(final_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# hack for buggy CMake's FindwxWidgets script
if (DEFINED ENV_WX_CONFIG)
    set (ENV{WX_CONFIG} ${ENV_WX_CONFIG})
endif()

set(wxWidgets_USE_STATIC 1)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/game/ DESTINATION bin	)

find_package(wxWidgets REQUIRED)

set(
	SRCS
	cMain.cpp
	cMain.h
	cApp.cpp
	cApp.h 
) 

include(${wxWidgets_USE_FILE})

if (APPLE)
    # create bundle on apple compiles
    add_executable(cMain MACOSX_BUNDLE ${SRCS} )
    # Set a custom plist file for the app bundle - needed for Mac OS Retina display
    set_target_properties(cMain PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
    # the WIN32 is needed for Windows in order for it to look for WinMain
    # instead of the cMain function. This is ignored on other systems,
    # so it works on all platforms
    add_executable(cMain WIN32 ${SRCS})
endif()

target_link_libraries(cMain PRIVATE ${wxWidgets_LIBRARIES})

install(TARGETS cMain DESTINATION bin)
