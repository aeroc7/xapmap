# fastfloat version 3.4.0
set(DEP_FASTFLOAT_VERSION "3.4.0")
set(DEP_FASTFLOAT_LOC "https://github.com/aeroc7/fast_float.git")

include(ExternalProject)
ExternalProject_Add(fastfloat-build
    GIT_REPOSITORY      ${DEP_FASTFLOAT_LOC}
    GIT_TAG             main
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       ""
    INSTALL_COMMAND     ""
)

ExternalProject_Get_Property(fastfloat-build SOURCE_DIR)


add_library(fastfloat INTERFACE)
target_include_directories(fastfloat INTERFACE ${SOURCE_DIR}/include)

add_dependencies(fastfloat fastfloat-build)