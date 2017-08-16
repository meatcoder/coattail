#include <ruby.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

//sleep interval in microseconds
#define SLEEP_INTERVAL 250000

static int open_file(VALUE obj) {
    //get filename from object
    VALUE fname_val = rb_iv_get(obj, "@fname");
    char* fname = StringValueCStr(fname_val);

    //actually open file
    int fd = open(fname, O_RDONLY);
    if (fd == -1) rb_raise(rb_eRuntimeError, "Unable to open file: %s (%s).", fname, strerror(errno));

    return fd;
}

static void read_lines(int fd, off_t* pos) {

}

static VALUE each_line(VALUE self) {
    //look for block param
    if (!rb_block_given_p()) rb_raise(rb_eArgError, "Block argument is required.");

    //open file
    int fd = open_file(self);
    if (fd < 0) return Qnil;

    //seek to end of file
    off_t cur_pos = lseek(fd, 0, SEEK_END);
    if (cur_pos == -1) rb_raise(rb_eRuntimeError, "Unable seek to end of file (%s).", strerror(errno));

    while (1) {
        //get file size
        struct stat info;
        int res = fstat(fd, &info);
        if (res == -1) rb_raise(rb_eRuntimeError, "Unable get size of file (%s).", strerror(errno));

        //check if the size of the file changed from earlier
        if (info.st_size > cur_pos) read_lines(fd, &cur_pos);
        else usleep(SLEEP_INTERVAL);
    }

    return Qtrue;
}

void Init_coattail(void) {
    VALUE cCoatTail = rb_const_get(rb_cObject, rb_intern("CoatTail"));
    rb_define_method(cCoatTail, "each_line", each_line, 0);
}
