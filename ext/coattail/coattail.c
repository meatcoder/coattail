#include <ruby.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>

#define SLEEP_INTERVAL 250000 /*sleep interval in microseconds*/
#define BUF_SIZE (8*1024) /*read buffer size*/
#define MAX_LINE_LENGTH (64*1024) /*max line length*/

typedef struct {
    char data[MAX_LINE_LENGTH+1];
    char* cur;
    int len;
} Line;

static VALUE cLine = Qnil;

static int open_file(VALUE obj) {
    //get filename from object
    VALUE fname_val = rb_iv_get(obj, "@fname");
    char* fname = StringValueCStr(fname_val);

    //actually open file
    int fd = open(fname, O_RDONLY);
    if (fd == -1) rb_raise(rb_eRuntimeError, "Unable to open file: %s (%s).", fname, strerror(errno));

    return fd;
}

static void Line_free(void* l) {
    xfree(l);
}

static void read_lines(VALUE obj, int fd, off_t* pos) {
    //get line struct
    VALUE line_obj = rb_iv_get(obj, "@line");
    Line* line = NULL;
    if (line_obj == Qnil) {
        line = ALLOC(Line);
        line->cur = line->data;
        line->len = 0;

        rb_iv_set(obj,"@line", Data_Wrap_Struct(cLine, NULL, Line_free,line));
    }
    else {
        Data_Get_Struct(line_obj, Line, line);
    }

    //read loop
    char buf[BUF_SIZE];
    while (1) {
        //read from file
        ssize_t num_read = read(fd, buf, BUF_SIZE);
        if (num_read == -1) rb_raise(rb_eRuntimeError, "Error reading from file (%s).", strerror(errno));
        if (num_read == 0) return; //end of file case
        *pos += num_read;

        //process read data
        char* b = buf;
        for (int i=0; i<num_read; i++, b++) {
            if ((*b == '\n') || (line->len >= MAX_LINE_LENGTH)) {
                if (line->len != 0) {
                    *(line->cur) = 0;
                    rb_yield(rb_str_new2(line->data));
                    line->cur = line->data;
                    line->len = 0;
                }
            }
            else {
                //copy data to line buffer and increase pointers
                *(line->cur) = *b;
                (line->cur)++;
                (line->len)++;
            }
        }
    }
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
        if (info.st_size > cur_pos) read_lines(self, fd, &cur_pos);
        else usleep(SLEEP_INTERVAL);
    }

    return Qtrue;
}

void Init_coattail(void) {
    VALUE cCoatTail = rb_const_get(rb_cObject, rb_intern("CoatTail"));
    rb_define_method(cCoatTail, "each_line", each_line, 0);

    cLine = rb_const_get(rb_cObject, rb_intern("CoatTailLine"));
}
