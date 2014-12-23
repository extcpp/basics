CPPFLAGS =-std=c++14 -I.. -lpthread

#below all example files are listed
define unstripped_files =
main_functional
main_function_cache
main_scoped_timer
main_type_to_string
endef
#main_multi_array

files = $(addprefix Examples/, $(strip $(unstripped_files)))

all: $(files)
