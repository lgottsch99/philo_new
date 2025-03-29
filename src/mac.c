#include <time.h>
#include "../includes/philos.h"

//usleep doesnt work on mac
void my_sleep(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;  // Convert ms to seconds
    ts.tv_nsec = (milliseconds % 1000) * 1000000;  // Remaining ns
    nanosleep(&ts, NULL);
}
