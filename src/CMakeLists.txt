set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# Source file storage
add_library(project_source INTERFACE)

# Include util directory
target_include_directories(project_source INTERFACE "${CMAKE_CURRENT_LIST_DIR}/")

add_subdirectory(config)
add_subdirectory(graphics)
add_subdirectory(parsers)
add_subdirectory(utils)

# Standalone executable
add_executable(${CMAKE_PROJECT_NAME}_standalone main.cpp xapmap.cpp)

# Link our project libraries
add_library(project_libraries INTERFACE)

# X-Plane path
option(XPLANE_ROOT "Path to your X-Plane root directory" REQUIRED)
if(NOT XPLANE_ROOT)
    message(FATAL_ERROR "Please set an X-Plane root directory with XPLANE_ROOT=[dir]")
else()
    target_compile_definitions(project_options INTERFACE -DUSER_XPLANE_ROOT="${XPLANE_ROOT}")
endif()

option(XPLANE_LIBRARY "Build the X-Plane library in addition to the executable")
# Restrict compiler to not use GCC for library builds (at least for now)
# In short, GCC uses unique symbols to prevent symbol conflicts
# with certain global constants in a particular library.
# This in turn can prevent dlclose() from being called,
# and we NEED that to be called for our plugin to get a
# reload without completely restarting X-Plane (only on Linux systems)
if(UNIX AND NOT APPLE AND XPLANE_LIBRARY)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        message(FATAL_ERROR "GCC compiler not supported for library builds on this platform")
    endif()
endif()

# Various library definitions
target_compile_definitions(project_options INTERFACE
    -DGLEW_STATIC
    -DM_PI=3.1415926535897932
    -DLOGGER_HEADER="${CMAKE_PROJECT_NAME}"
    -DGL_SILENCE_DEPRECATION
    )

if(WIN32)
    target_link_libraries(project_libraries
        INTERFACE
            -static
            -static-libgcc
            -static-libstdc++
    )
elseif(UNIX AND NOT APPLE)
   target_link_libraries(project_libraries
        INTERFACE
            -static-libgcc
            -static-libstdc++
            GL
   )
elseif(APPLE)
    target_link_libraries(project_libraries
        INTERFACE
            "-framework OpenGL"
    )
endif()

target_link_libraries(${CMAKE_PROJECT_NAME}_standalone
    PRIVATE
        project_source
        project_options
        project_warnings
        project_libraries
        Threads::Threads
        cairo
        freetype
        pixman
        glew
        glfw
        xpsdk
        fastfloat
        nuklear
        fonts
        -lm
)