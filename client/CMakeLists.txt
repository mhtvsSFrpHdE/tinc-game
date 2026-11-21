cmake_minimum_required(VERSION "3.15")

# C++ 14
set(CMAKE_CXX_STANDARD 14)
# Static link /MT, MTd
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Boost
set(BOOST_ROOT $ENV{CPP_DEVEL_BOOST})

# wxWidgets
set(wxBUILD_SHARED OFF)
set(wxWidgets_ROOT_DIR "$ENV{CPP_DEVEL_WXWIN}")
set(wxWidgets_LIB_DIR "$ENV{CPP_DEVEL_WXWIN}/build_x64/lib/vc_x64_lib")

project(resource)

# Boost
find_package(Boost REQUIRED)

# wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

add_library("${CMAKE_PROJECT_NAME}")
file(GLOB_RECURSE RESOURCE_SOURCE_FILE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/resource/*.cpp")
target_sources("${CMAKE_PROJECT_NAME}"
    PRIVATE
        "${RESOURCE_SOURCE_FILE}"
)
# Library added by `find_package` doesn't need `target_include_directories`
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE
    Boost::headers
    ${wxWidgets_LIBRARIES})
