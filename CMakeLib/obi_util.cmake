
#! prefix string with provided symbol(s) until is has given length
#
#  in_string - sting to be prefixed
#  length - desired length
#  fill - symbols(s) used for filling
#  out_string - this will hold the result
function(prefix in_string length fill out_string)
    set(result "${in_string}")
    string(LENGTH "${in_string}" current_length)

    while(current_length LESS length)
        set(result "${fill}${result}")
        string(LENGTH "${result}" current_length)
    endwhile()

    set("${out_string}" "${result}" PARENT_SCOPE)
endfunction(prefix)

#! this function acts like add_subdirectory but it checks
#  if CMakeLists.txt exists in the directory before adding it
function(add_subdirectory_checked dir debug)
    if(EXISTS "${dir}/CMakeLists.txt")
        add_subdirectory("${dir}")
        if(debug)
            message("adding directory ${dir}")
        endif()
    endif()
endfunction()
