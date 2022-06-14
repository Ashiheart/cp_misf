#pragma once

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

//#define TIMER_SIMPLE
//#define TIMER_VALUE_ONLY
//#define TIMER_DEBUG

#ifdef TIMER_SIMPLE
#define function_timer(method, message) { \
    struct timeval start, end; \
    gettimeofday(&start, NULL); \
    method; \
    gettimeofday(&end, NULL); \
    fprintf(stderr, "%s:%f ", message, ((end.tv_sec - start.tv_sec) * 1000. + (end.tv_usec - start.tv_usec) * 0.001)); }
#endif

#ifdef TIMER_DEBUG
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

#ifdef TIMER_VALUE_ONLY
#define function_timer(method, message) { \
    struct timeval start, end; \
    gettimeofday(&start, NULL); \
    method; \
    gettimeofday(&end, NULL); \
    fprintf(stderr, "%f ", ((end.tv_sec - start.tv_sec) * 1000. + (end.tv_usec - start.tv_usec) * 0.001)); }
#endif

#ifndef TIMER_SIMPLE
#ifndef TIMER_DEBUG
#ifndef TIMER_VALUE_ONLY
#define function_timer(method, message) method
#define timer_print(str, start, end) 
#endif
#endif
#endif

#undef TIMER_VALUE_ONLY
#undef TIMER_SIMPLE
#undef TIMER_DEBUG


