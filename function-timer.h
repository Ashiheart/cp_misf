#pragma once

#include <unistd.h>
#include <sys/time.h>

//#define MEASURE_MODE

#ifdef MEASURE_MODE
#define timer_print(str, start, end) \
    fprintf(stderr, "%d, %s: %s:\texe time %f[ms]\n", \
            __LINE__, __func__, str, \
            ((end.tv_sec - start.tv_sec) * 1000. + (end.tv_usec - start.tv_usec) * 0.001))

#define function_timer(method, message) { \
    struct timeval start, end; \
    gettimeofday(&start, NULL); \
    method; \
    gettimeofday(&end, NULL); \
    timer_print(message, start, end); }
#else
#define function_timer(method, message) method
#endif

