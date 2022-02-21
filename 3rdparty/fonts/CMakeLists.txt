add_library(fonts INTERFACE)

include(ExternalProject)
function(get_font_resource url font_name)
    ExternalProject_Add(${font_name}-build
        URL                 ${url}
        CONFIGURE_COMMAND   ""
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ""
    )

    ExternalProject_Get_Property(${font_name}-build SOURCE_DIR)
    add_dependencies(fonts ${font_name}-build)
endfunction()

get_font_resource("https://fonts.google.com/download?family=Roboto" "roboto")