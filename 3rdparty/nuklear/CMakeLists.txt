# nuklear version 4.9.6
set(DEP_NUKLEAR_VERSION "4.9.6")
set(DEP_NUKLEAR_LOC "https://github.com/Immediate-Mode-UI/Nuklear/archive/refs/tags/4.9.6.zip")

include(ExternalProject)

ExternalProject_Add(nuklear-build
    URL                 ${DEP_NUKLEAR_LOC}
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       cd <SOURCE_DIR>/src/ && sh paq.sh
    INSTALL_COMMAND     cp <SOURCE_DIR>/src/nuklear.h <INSTALL_DIR>/include/nuklear
)

ExternalProject_Get_Property(nuklear-build INSTALL_DIR)

file(MAKE_DIRECTORY ${INSTALL_DIR}/include/nuklear)

add_library(nuklear INTERFACE)
target_include_directories(nuklear INTERFACE ${INSTALL_DIR}/include)

add_dependencies(nuklear nuklear-build)