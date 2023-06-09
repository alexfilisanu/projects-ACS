#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <internal/syscall.h>

int nanosleep(const struct timespec *req, struct timespec *rem) {
    int ret = syscall(__NR_nanosleep, req, rem);
    if (ret < 0) {
        errno = -ret;
        return (*rem).tv_sec;
    }

    return 0;
}