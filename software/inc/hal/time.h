#pragma once
#include "types.h"

#define TIMER_FREQUENCY 32768UL

// microseconds
typedef u64 timespan_t;

#define TIMESPAN_TO_MICROSECONDS(t)     (t)
#define TIMESPAN_TO_MILLISECONDS(t)     (t / (1000UL))
#define TIMESPAN_TO_SECONDS(t)          (t / (1000UL * 1000UL))
#define TIMESPAN_TO_MINUTES(t)          (t / (1000UL * 1000UL * 60UL))
#define TIMESPAN_TO_HOURS(t)            (t / (1000UL * 1000UL * 60UL * 60UL))

#define TIMESPAN_FROM_MICROSECONDS(t)   (t)
#define TIMESPAN_FROM_MILLISECONDS(t)   (t * (1000UL))
#define TIMESPAN_FROM_SECONDS(t)        (t * (1000UL * 1000UL))
#define TIMESPAN_FROM_MINUTES(t)        (t * (1000UL * 1000UL * 60UL))
#define TIMESPAN_FROM_HOURS(t)          (t * (1000UL * 1000UL * 60UL * 60UL))

void time_init(void);

timespan_t time_elapsed(void);
timespan_t time_elapsed_since(timespan_t start);
void time_wait_for(timespan_t duration);
void time_wait_until(timespan_t timestamp);