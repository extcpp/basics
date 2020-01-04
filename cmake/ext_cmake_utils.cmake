#! some basic settings i use really often so lets have them in a macro

function(ext_log)
    message(STATUS "extINFO -- " ${ARGV})
endfunction(ext_log)

function(ext_fatal)
    message(FATAL_ERROR "FATAL ERROR -- " ${ARGV})
endfunction(ext_fatal)

macro(ext_add_test_subdirectory type)
    set(EXT_TEST_TYPE "${type}")

    set(dir "${ARGV1}")
    if(dir STREQUAL "")
        set(dir "tests")
    endif()

    if("${type}" STREQUAL "google") # <-- expand type here!
        if(NOT TARGET gtest) #avoid recursive inclusion of gtest
          if("${LIBEXT_SOURCE_DIR}" STREQUAL "")
              ext_fatal("LIBEXT_SOURCE_DIR not found")
          endif()
          ext_log("using google test in: ${LIBEXT_SOURCE_DIR}/external_libs/googletest")
          add_subdirectory(${LIBEXT_SOURCE_DIR}/external_libs/googletest)
        endif()
    else()
        message(ERROR "unknown test type")
    endif()
    ext_log("adding tests in: ${dir}")
    add_subdirectory("${dir}")
endmacro(ext_add_test_subdirectory)

#! prefix string with provided symbol(s) until is has given length
#
#  in_string - sting to be prefixed
#  length - desired length
#  fill - symbols(s) used for filling
#  out_string - this will hold the result
function(ext_prefix in_string length fill out_string)
    set(result "${in_string}")
    string(LENGTH "${in_string}" current_length)

    while(current_length LESS length)
        set(result "${fill}${result}")
        string(LENGTH "${result}" current_length)
    endwhile()

    set("${out_string}" "${result}" PARENT_SCOPE)
endfunction(ext_prefix)

#! this function acts like add_subdirectory but it checks
#  if CMakeLists.txt exists in the directory before adding it
function(ext_add_subdirectory dir debug)
    if(EXISTS "${dir}/CMakeLists.txt")
        add_subdirectory("${dir}")
        if(debug)
            message("adding directory ${dir}")
        endif()
    endif()
endfunction()

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
