#include <ruby.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

static int open_file(VALUE obj) {
    //get filename from object
    VALUE fname_val = rb_iv_get(obj, "@fname");
    char* fname = StringValueCStr(fname_val);

    //actually open file
    int fd = open(fname, O_RDONLY);
    if (fd == -1) rb_raise(rb_eRuntimeError, "Unable to open file: %s (%s).", fname, strerror(errno));

    return fd;
}

static VALUE each_line(VALUE self) {
    //look for block param
    if (!rb_block_given_p()) rb_raise(rb_eArgError, "Block argument is required.");

    //open file
    int fd = open_file(self);
    if (fd < 0) return Qnil;

    return Qnil;
}

void Init_coattail(void) {
    VALUE cCoatTail = rb_const_get(rb_cObject, rb_intern("CoatTail"));
    rb_define_method(cCoatTail, "each_line", each_line, 0);
}
