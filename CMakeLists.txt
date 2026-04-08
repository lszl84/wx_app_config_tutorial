cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(settings_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(wxBUILD_SHARED OFF)

include(FetchContent)

message(STATUS "Fetching wxWidgets...")

FetchContent_Declare(
    wxWidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(wxWidgets)

set(SRCS
    src/app.cpp
    src/main_frame.cpp
)

if(APPLE)
    add_executable(main MACOSX_BUNDLE ${SRCS})
    set_target_properties(main PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
elseif(WIN32)
    add_executable(main WIN32 ${SRCS} main.exe.manifest resources.rc)
else()
    add_executable(main ${SRCS})
endif()

target_link_libraries(main PRIVATE wxcore wxbase)
