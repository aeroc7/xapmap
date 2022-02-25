function(gen_bin_header file_dir)
    file(GLOB font_files_list "${file_dir}")

    foreach(font_file IN LISTS font_files_list)
        get_filename_component(raw_file_name_ne ${font_file} NAME_WLE)
        get_filename_component(raw_file_name_e ${font_file} NAME)
        get_filename_component(raw_file_dir ${font_file} DIRECTORY)
        
        if(NOT EXISTS "${raw_file_dir}/${raw_file_name_ne}.h")
            message(STATUS "\t ${raw_file_name_e} --> ${raw_file_name_ne}.h")
            include(${PARAMETER_REF_DIR}/cmake/BinToHeader.cmake)
            bin2h(SOURCE_FILE ${font_file} HEADER_FILE ${raw_file_dir}/${raw_file_name_ne}.h VARIABLE_NAME ${raw_file_name_ne})
        endif()
    endforeach()
endfunction()


if(NOT PARAMETER_FILES_DIR)
    message(FATAL_ERROR "Must provide files parameter")
endif()

if(NOT PARAMETER_REF_DIR)
    message(FATAL_ERROR "Must provide reference directory")
endif()

gen_bin_header(${PARAMETER_FILES_DIR})