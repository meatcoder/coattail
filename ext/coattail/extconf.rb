require "mkmf"

abort "no fcntl.h" unless  have_header "fcntl.h"
abort "no string.h" unless  have_header "string.h"
abort "no errno.h" unless  have_header "errno.h"
abort "no unistd.h" unless  have_header "unistd.h"
abort "no sys/stat.h" unless  have_header "sys/stat.h"
abort "no sys/types.h" unless  have_header "sys/types.h"
abort "no sys/uio.h" unless  have_header "sys/uio.h"

create_makefile "coattail/coattail"
