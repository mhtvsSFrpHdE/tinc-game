cmake_minimum_required(VERSION "3.15")

# C++ 14
set(CMAKE_CXX_STANDARD 14)
# Static link /MT, MTd
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Solution tinc-game
project(tinc-game)

# Boost
set(BOOST_ROOT "$ENV{CPP_DEVEL_BOOST}")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost REQUIRED container filesystem)

# wxWidgets
set(wxBUILD_SHARED OFF)
set(wxWidgets_ROOT_DIR "$ENV{CPP_DEVEL_WXWIN}")
message(STATUS "CMAKE_CXX_COMPILER_ARCHITECTURE_ID: ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
if("x64" STREQUAL CMAKE_CXX_COMPILER_ARCHITECTURE_ID)
    set(wxWidgets_LIB_DIR "$ENV{CPP_DEVEL_WXWIN}/build_x64/lib/vc_x64_lib")
elseif("X86" STREQUAL CMAKE_CXX_COMPILER_ARCHITECTURE_ID)
    set(wxWidgets_LIB_DIR "$ENV{CPP_DEVEL_WXWIN}/build_win32/lib/vc_lib")
endif()
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

# cpr
set(BUILD_SHARED_LIBS OFF)
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.9.9) # The commit hash for 1.11.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

# Project resource
add_library("resource")
file(GLOB_RECURSE RESOURCE_SOURCE_FILE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/resource/*.cpp")
target_sources("resource"
    PRIVATE
        "${RESOURCE_SOURCE_FILE}"
)
target_link_libraries("resource" PRIVATE
    Boost::headers
    ${wxWidgets_LIBRARIES}
)

# Project tinc-game
add_executable("tinc-game" WIN32)
file(GLOB_RECURSE TINC_GAME_SOURCE_FILE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tinc-game/src/*.cpp")
target_sources("tinc-game"
    PRIVATE
        "${TINC_GAME_SOURCE_FILE}"
)
target_include_directories("tinc-game" PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries("tinc-game" PRIVATE
    resource
    Boost::headers
    Boost::filesystem
    Boost::container
    ${wxWidgets_LIBRARIES}
    cpr::cpr
)

# Project updater
add_executable("updater" WIN32)
file(GLOB_RECURSE UPDATER_SOURCE_FILE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/updater/*.cpp")
target_sources("updater"
    PRIVATE
        "${UPDATER_SOURCE_FILE}"
)
target_include_directories("updater" PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries("updater" PRIVATE
    resource
    Boost::headers
    Boost::filesystem
    ${wxWidgets_LIBRARIES}
)
