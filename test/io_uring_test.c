#include <linux/io_uring.h>
#include <linux/fs.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/uio.h>

#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <liburing.h>

int main() {
    fprintf(stdout, "io_uring is complicated, liburing simplify it.\n");
    return 0;
}