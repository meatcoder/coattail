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
    if (fd == -1) rb_raise(rb_eRuntimeError, "Unable to open file: %s (%s)", fname, strerror(errno));

    return fd;
}

//static void get_last_lines(int fd, int num_lines)

static VALUE each_line_ext(VALUE self, VALUE num_lines, VALUE wait_indefinitely, VALUE blk) {
    int fd = open_file(self);
    if (fd < 0) return Qnil;

    //if (NUM2INT(num_lines) != 0)
    return Qnil;
}

void Init_coattail(void) {
    VALUE cCoatTail = rb_const_get(rb_cObject, rb_intern("CoatTail"));
    rb_define_method(cCoatTail, "each_line_ext", each_line_ext, 3);
}
