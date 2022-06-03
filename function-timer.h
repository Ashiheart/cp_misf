#pragma once

#include <unistd.h>
#include <sys/time.h>

#undef SIMPLE_MODE
#undef MEASURE_MODE

//#define SIMPLE_MODE
#define VALUE_ONLY_MODE
//#define MEASURE_MODE

#ifdef SIMPLE_MODE
#define function_timer(method, message) { \
    struct timeval start, end; \
    gettimeofday(&start, NULL); \
    method; \
    gettimeofday(&end, NULL); \
    fprintf(stderr, "%s:%f ", message, ((end.tv_sec - start.tv_sec) * 1000. + (end.tv_usec - start.tv_usec) * 0.001)); }
#endif

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
#endif

#ifdef VALUE_ONLY_MODE
#define function_timer(method, message) { \
    struct timeval start, end; \
    gettimeofday(&start, NULL); \
    method; \
    gettimeofday(&end, NULL); \
    fprintf(stderr, "%f ", ((end.tv_sec - start.tv_sec) * 1000. + (end.tv_usec - start.tv_usec) * 0.001)); }
#endif

#ifndef SIMPLE_MODE
#ifndef MEASURE_MODE
#ifndef VALUE_ONLY_MODE
#define function_timer(method, message) method
#define timer_print(str, start, end) 
#endif
#endif
#endif

