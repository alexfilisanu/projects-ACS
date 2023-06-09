#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <internal/syscall.h>

unsigned int sleep(unsigned int seconds)
{
    struct timespec ts = { .tv_sec = seconds, .tv_nsec = 0 };
    
    return nanosleep(&ts, NULL);
}