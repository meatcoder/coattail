#include <ruby.h>

static VALUE each_line_ext(VALUE self, VALUE num_lines, VALUE wait_indefinitely, VALUE blk) {
  return Qnil;
}

void Init_coattail(void) {
  VALUE cCoatTail = rb_const_get(rb_cObject, rb_intern("CoatTail"));
  rb_define_method(cCoatTail, "each_line_ext", each_line_ext, 3);
}
