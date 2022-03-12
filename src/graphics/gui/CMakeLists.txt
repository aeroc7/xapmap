target_sources(project_source INTERFACE
    gui_fonts.cpp
    gui_main.cpp
)

add_subdirectory(nuklear_impl)
add_subdirectory(map)
add_subdirectory(windows)