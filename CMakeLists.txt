cmake_minimum_required(VERSION 3.17)

project(xapmap CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Restrict compiler to not use GCC (at least for now)
# In short, GCC uses unique symbols to prevent conflict
# with certain global constants in a particular library.
# This in turn can prevent dlclose() from being called,
# and we NEED that to be called for our plugin to get a
# reload without completely restarting X-Plane
if(UNIX AND NOT APPLE)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        message(FATAL_ERROR "GCC compiler not supported")
    endif()
endif()

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_POSITION_INDEPENDENT_CODE 1)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

# Compiler warnings
add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Sanitizers
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# Static analysis
include(cmake/StaticAnalyzers.cmake)

add_subdirectory(3rdparty)
add_subdirectory(src)
