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

#define MSEC (1000UL)
#define USEC (1000000UL)
#define NSEC (1000000000UL)

#define SEC_TO_MSEC(__sec__)   ((__sec__) * MSEC)
#define SEC_TO_USEC(__sec__)   ((__sec__) * USEC)
#define SEC_TO_NSEC(__sec__)   ((__sec__) * NSEC)

#define USEC_TO_SEC(__nsec__)  ((double)(__nsec__) / (double)USEC)
#define USEC_TO_MSEC(__nsec__)  ((double)(__nsec__) / (double)(USEC/MSEC))
#define USEC_TO_NSEC(__nsec__) ((__nsec__) * (NSEC/USEC))

#define NSEC_TO_SEC(__nsec__)  ((double)(__nsec__) / (double)NSEC)
#define NSEC_TO_MSEC(__nsec__)  ((double)(__nsec__) / (double)(NSEC/MSEC))
#define NSEC_TO_USEC(__nsec__) ((double)(__nsec__) / (double)(NSEC/USEC))


static inline struct timespec get_timespec() {
    struct timespec ret;
    int res = clock_gettime(CLOCK_REALTIME, &ret);
    if (res != 0) {
        fprintf(stderr, "clock_gettime(CLOCK_REALTIME,..): %s\n", strerror(errno));
        exit(1);
    }
    return ret;
}


static inline struct timeval get_timeval() {
    struct timeval ret;
    int res = gettimeofday(&ret, NULL);
    if (res != 0) {
        fprintf(stderr, "gettimeofday(..): %s\n", strerror(errno));
        exit(1);
    }
    return ret;
}


static inline struct timeval timeval_subtract(struct timeval start_time, struct timeval end_time) {
    struct timeval result;
    /* Perform the carry for the later subtraction by updating y. */
    if (end_time.tv_usec < start_time.tv_usec) {
        int nsec = (start_time.tv_usec - end_time.tv_usec) / USEC + 1;
        start_time.tv_usec -= 1000000 * nsec;
        start_time.tv_sec += nsec;
    }
    if (end_time.tv_usec - start_time.tv_usec > 1000000) {
        int nsec = (end_time.tv_usec - start_time.tv_usec) / USEC;
        start_time.tv_usec += 1000000 * nsec;
        start_time.tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
    result.tv_sec = end_time.tv_sec - start_time.tv_sec;
    result.tv_usec = end_time.tv_usec - start_time.tv_usec;

    return result;
}


static inline struct timespec timespec_subtract(struct timespec start, struct timespec end) {
    struct timespec result;
    if (start.tv_nsec < end.tv_nsec) {
        result.tv_sec = end.tv_sec - start.tv_sec - 1;
        result.tv_nsec = NSEC + end.tv_nsec - start.tv_nsec;
    } else {
        result.tv_sec = end.tv_sec - start.tv_sec;
        result.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return result;
}


static inline double timeval_subtract_sec(struct timeval start_time, struct timeval end_time) {
    struct timeval result = timeval_subtract(start_time, end_time);
    return (double) result.tv_sec + USEC_TO_SEC(result.tv_usec);
}

static inline uint64_t timeval_subtract_msec(struct timeval start_time, struct timeval end_time) {
    struct timeval result = timeval_subtract(start_time, end_time);
    return ((uint64_t) SEC_TO_MSEC(result.tv_sec)) + (uint64_t) USEC_TO_MSEC(result.tv_usec);
}


static inline uint64_t timeval_subtract_usec(struct timeval start_time, struct timeval end_time) {
    struct timeval result = timeval_subtract(start_time, end_time);
    return ((uint64_t) result.tv_sec * USEC) + (uint64_t) result.tv_usec;
}

#endif /* CLIENTS_TIMEVAL_DIFF_H_ */
