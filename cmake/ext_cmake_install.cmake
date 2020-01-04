macro(ext_install lib)
    install(
        TARGETS "${lib}"
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
    )

    set(args "${ARGN}")
    foreach(arg IN LISTS ${args})
        message("${arg}")
        install(
            DIRECTORY   "${arg}"
            DESTINATION include
        )
    endforeach()

    include(CPack)
endmacro(ext_install)


function(ext_install_files from_dir to_dir files)
	foreach (file ${files})
	    get_filename_component(parent ${file} DIRECTORY)
	    if(IS_DIRECTORY ${from_dir}/${file})
		    ext_log("install dir ${from_dir}/${file} to ${to_dir}/${parent}")
	        install(
	            DIRECTORY
	                ${from_dir}/${file}
	            DESTINATION
	                ${to_dir}/${parent}
	          )
	    else()
		    ext_log("install file ${from_dir}/${file} to ${to_dir}/${parent}")
	        install(
	            FILES
	                ${from_dir}/${file}
	            DESTINATION
	                ${to_dir}/${parent}
	        )
	    endif()
	endforeach()
endfunction(ext_install_files)
