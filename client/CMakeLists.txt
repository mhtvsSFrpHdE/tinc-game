cmake_minimum_required(VERSION "3.15")

# C++ 11
set(CMAKE_CXX_STANDARD 11)
# Static link Visual Studio /MT, MTd
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Solution tinc-game
project(tinc-game)

# Boost
if(MINGW)
    # Fix 1.75.0 boost/process not compile
    add_definitions(-D__kernel_entry=)
endif()
set(BOOST_ROOT "$ENV{CPP_DEVEL_BOOST}")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost REQUIRED container filesystem)

# wxWidgets
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
set(CPR_VERSION "1.4.0")
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG ${CPR_VERSION}) # The commit hash for 1.11.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
if(NOT TARGET cpr::cpr)
    add_library(cpr::cpr ALIAS cpr)
endif()

# MinGW Windows exe admin permission manifest
set(APP_MANIFEST_RC "${CMAKE_CURRENT_SOURCE_DIR}/resource/app.rc")

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
    iphlpapi
)
# manifest admin permission
if(MSVC AND WIN32 AND CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo|MinSizeRel")
    target_link_options("tinc-game" PRIVATE
        "/MANIFESTUAC:level='requireAdministrator' uiAccess='false'"
    )
elseif(MINGW AND WIN32 AND CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo|MinSizeRel")
    target_sources("tinc-game" PRIVATE ${APP_MANIFEST_RC})
endif()

# Project updater
add_executable("updater" WIN32)
file(GLOB_RECURSE UPDATER_SOURCE_FILE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/updater/*.cpp")
target_sources("updater"
    PRIVATE
        "${UPDATER_SOURCE_FILE}"
        ${APP_MANIFEST_RC}
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
# manifest admin permission
if(MSVC AND WIN32 AND CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo|MinSizeRel")
    target_link_options("updater" PRIVATE
        "/MANIFESTUAC:level='requireAdministrator' uiAccess='false'"
    )
elseif(MINGW AND WIN32 AND CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo|MinSizeRel")
    target_sources("updater" PRIVATE ${APP_MANIFEST_RC})
endif()
