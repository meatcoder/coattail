require "mkmf"

abort "no fcntl.h" unless  have_header "fcntl.h"
abort "no string.h" unless  have_header "string.h"
abort "no errno.h" unless  have_header "errno.h"
abort "no unistd.h" unless  have_header "unistd.h"
abort "no sys/stat.h" unless  have_header "sys/stat.h"

create_makefile "coattail/coattail"
