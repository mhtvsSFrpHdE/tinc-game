cmake_minimum_required(VERSION "3.15")

# C++ 11
set(CMAKE_CXX_STANDARD 11)

# Solution tinc-game
project(tinc-game)

if(MSVC)
    # Static link
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(MINGW)
    # Static link
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    # Target Windows XP 2001 (No Service Packs required)
    set(_WIN32_WINNT 0x0501)
endif()

# Boost
if(MINGW)
    # Set Boost_ARCHITECTURE is required to locate correct lib files when using legacy compiler
    set(Boost_ARCHITECTURE -x32)
    # Fix 1.75.0 boost/process not compile
    add_definitions(-D__kernel_entry=)
endif()
set(BOOST_ROOT "$ENV{CPP_DEVEL_BOOST}")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost REQUIRED container filesystem locale)

# wxWidgets
set(wxWidgets_ROOT_DIR "$ENV{CPP_DEVEL_WXWIN}")
message(STATUS "CMAKE_CXX_COMPILER_ARCHITECTURE_ID: ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
if(MSVC AND "x64" STREQUAL CMAKE_CXX_COMPILER_ARCHITECTURE_ID)
    set(wxWidgets_LIB_DIR "$ENV{CPP_DEVEL_WXWIN}/lib/vc_x64_lib")
elseif(MSVC AND "X86" STREQUAL CMAKE_CXX_COMPILER_ARCHITECTURE_ID)
    set(wxWidgets_LIB_DIR "$ENV{CPP_DEVEL_WXWIN}/lib/vc_lib")
endif()
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

# cpr
# Version 1.5.2, WINSSL, TEST OFF is last one compile on MinGW
set(BUILD_SHARED_LIBS OFF)
set(BUILD_CPR_TESTS OFF)
set(USE_OPENSSL OFF)
set(USE_WINSSL ON)
if(MINGW)
    # Fix cpr source code (curl) _WIN32_WINNT not follow override
    set(ENABLE_INET_PTON OFF)
endif()
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.5.2) # The commit hash for 1.11.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
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
if(MSVC)
    # Target Windows 7 (SP1 may required)
    target_compile_definitions("resource" PRIVATE _WIN32_WINNT=0x0601)
endif()

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
    Boost::locale
    ${wxWidgets_LIBRARIES}
    cpr::cpr
    iphlpapi
    setupapi
)
if(MSVC)
    # Target Windows 7 (SP1 may required)
    target_compile_definitions("tinc-game" PRIVATE _WIN32_WINNT=0x0601)
endif()
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
if(MSVC)
    # Target Windows 7 (SP1 may required)
    target_compile_definitions("updater" PRIVATE _WIN32_WINNT=0x0601)
    # Fix CVT1100 duplicate resource
    target_link_options("updater" PRIVATE "/MANIFEST:NO")
endif()
# manifest admin permission
if(MSVC AND WIN32 AND CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo|MinSizeRel")
    target_link_options("updater" PRIVATE
        "/MANIFESTUAC:level='requireAdministrator' uiAccess='false'"
    )
elseif(MINGW AND WIN32 AND CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo|MinSizeRel")
    target_sources("updater" PRIVATE ${APP_MANIFEST_RC})
endif()
