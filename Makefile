# Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

CPPFLAGS =-std=c++14 -I.. -lpthread

#below all example files are listed
define unstripped_files =

functional
function_cache
scoped_timer
type_to_string
static_lambda
meta
load_library

endef
#main_multi_array

files = $(addprefix Examples/, $(strip $(unstripped_files)))
.PHONY: clean

all: $(files)
clean:
	rm -fr $(files)
