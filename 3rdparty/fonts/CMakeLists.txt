include(${CMAKE_SOURCE_DIR}/cmake/BinToHeader.cmake)

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

    add_custom_target(gen-${font_name}
        COMMAND ${CMAKE_COMMAND} 
            -D PARAMETER_REF_DIR=${CMAKE_SOURCE_DIR}
            -D PARAMETER_FILES_DIR="${SOURCE_DIR}/*.ttf"
            -P ${CMAKE_SOURCE_DIR}/cmake/GenBinHeader.cmake
        DEPENDS ${font_name}-build
        COMMENT "Generating ${font_name} binary headers..."
    )

    target_include_directories(fonts INTERFACE ${SOURCE_DIR})

    add_dependencies(fonts gen-${font_name})
endfunction()

get_font_resource("https://fonts.google.com/download?family=Roboto" "roboto")