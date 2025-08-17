#pragma once
#include "fifo.h"
#include "hal/time.h"

#define APP_DOUBLE_TAP_QUIET_TIME  (TIMESPAN_FROM_MILLISECONDS(300))
#define APP_TILT_ANGLE_LOCKIN_TIME (TIMESPAN_FROM_SECONDS(1))

typedef enum {

    APP_INPUT_NONE          = (1 << 0),
    APP_INPUT_SINGLE_TAP    = (1 << 1),
    APP_INPUT_DOUBLE_TAP    = (1 << 2),

} app_input_t;

typedef enum {

    APP_STATE_SLEEP = 0,
    APP_STATE_SHOW_TIME,
    APP_STATE_SHOW_TEMP,
    APP_STATE_SET_TIME_HOUR,
    APP_STATE_SET_TIME_MINUTE,
    APP_STATE_SET_TIME_SECOND,
    APP_STATE_COUNT

} app_state_type_t;

typedef struct {

    void(*on_enter)(void);
    void(*on_exit)(void);
    app_state_type_t(*tick)(void);

} app_state_t;

void app_run(void);
app_input_t app_peek_latest_input(void);
app_input_t app_pop_latest_input(void);
