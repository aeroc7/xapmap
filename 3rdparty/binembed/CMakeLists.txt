include(ExternalProject)
ExternalProject_Add(binembed-build
    GIT_REPOSITORY      "https://github.com/aeroc7/binembed.git"
    GIT_TAG             main
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       ${CMAKE_CXX_COMPILER} -o ${CMAKE_BINARY_DIR}/binembed -std=c++20 -O3 <SOURCE_DIR>/binembed.cpp
    INSTALL_COMMAND     ""
)