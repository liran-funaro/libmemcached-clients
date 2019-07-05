/*
 * timeval_diff.h
 *
 * Author: Liran Funaro <liran.funaro@gmail.com>
 */

#ifndef CLIENTS_TIMEVAL_DIFF_H_
#define CLIENTS_TIMEVAL_DIFF_H_

#include <limits.h>
#include <inttypes.h>
#include <time.h>

static inline struct timeval timeval_subtract(struct timeval start_time, struct timeval end_time) {
    struct timeval result;
    /* Perform the carry for the later subtraction by updating y. */
    if (end_time.tv_usec < start_time.tv_usec) {
        int nsec = (start_time.tv_usec - end_time.tv_usec) / 1000000 + 1;
        start_time.tv_usec -= 1000000 * nsec;
        start_time.tv_sec += nsec;
    }
    if (end_time.tv_usec - start_time.tv_usec > 1000000) {
        int nsec = (end_time.tv_usec - start_time.tv_usec) / 1000000;
        start_time.tv_usec += 1000000 * nsec;
        start_time.tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
    result.tv_sec = end_time.tv_sec - start_time.tv_sec;
    result.tv_usec = end_time.tv_usec - start_time.tv_usec;

    return result;
}


static inline double timeval_subtract_sec(struct timeval start_time, struct timeval end_time) {
    struct timeval result = timeval_subtract(start_time, end_time);
    return (double) result.tv_sec + ((double) result.tv_usec / 1000000);
}


static inline uint64_t timeval_subtract_usec(struct timeval start_time, struct timeval end_time) {
    struct timeval result = timeval_subtract(start_time, end_time);
    return ((uint64_t) result.tv_sec * 1000000) + (uint64_t) result.tv_usec;
}

#endif /* CLIENTS_TIMEVAL_DIFF_H_ */
