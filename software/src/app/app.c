#include "app/app.h"
#include "app/app_state_sleep.h"
#include "app/app_state_show_time.h"
#include "app/app_state_show_temp.h"
#include "app/app_state_set_time.h"
#include "hal/standby.h"
#include "hal/bma400.h"
#include "cmsis/stm32l071xx.h"

#define APP_STATE(name, interval) { \
            .on_enter = name##_on_enter, \
            .on_exit = name##_on_exit, \
            .tick = name##_tick, \
            .tick_interval = interval \
        }

static const app_state_t app_states[APP_STATE_COUNT] = {
    APP_STATE(app_state_sleep, TIMESPAN_FROM_MILLISECONDS(100U)),
    APP_STATE(app_state_show_time, TIMESPAN_FROM_MILLISECONDS(100U)),
    APP_STATE(app_state_show_temp, TIMESPAN_FROM_MILLISECONDS(100U)),
    APP_STATE(app_state_set_time_hour, TIMESPAN_FROM_MILLISECONDS(20U)),
    APP_STATE(app_state_set_time_minute, TIMESPAN_FROM_MILLISECONDS(20U)),
};

void app_run(void)
{
    // maybe we just woke up?
    if(standby_last_powerdown_was_standby() && app_states[APP_STATE_SLEEP].on_exit != NULL)
    {
        app_states[APP_STATE_SLEEP].on_exit();
    }

    if(app_states[APP_STATE_SHOW_TIME].on_enter != NULL)
    {
        app_states[APP_STATE_SHOW_TIME].on_enter();
    }

    app_state_type_t current_state = APP_STATE_SHOW_TIME;

    while(true)
    {
        app_state_type_t next_state = app_states[current_state].tick();
        timespan_t tick_delay = app_states[current_state].tick_interval;

        time_wait_for(tick_delay);

        if(next_state != current_state)
        {
            if(app_states[current_state].on_exit != NULL)
            {
                app_states[current_state].on_exit();
            }
            if(app_states[next_state].on_enter != NULL)
            {
                app_states[next_state].on_enter();
            }

            current_state = next_state;
        }
    }
}

app_input_t app_peek_latest_input(void)
{
    bma400_tap_event_t latest_event;
    bool event_occured = fifo_peek(&bma400_tap_queue, (u8*)&latest_event);
    if(!event_occured)
    {
        return APP_INPUT_NONE;
    }

    if(latest_event.type == BMA400_TAPTYPE_DOUBLE)
    {
        return APP_INPUT_DOUBLE_TAP;
    }
    else if(latest_event.type == BMA400_TAPTYPE_SINGLE)
    {
        if(time_elapsed_since(latest_event.timestamp) < APP_DOUBLE_TAP_QUIET_TIME)
        {
            // we're not sure whether this is going to be followed by a second tap.
            return APP_INPUT_NONE;
        }
        else
        {
            // there's no second tap within the configured window.
            return APP_INPUT_SINGLE_TAP;
        }
    }

    return APP_INPUT_NONE;
}

app_input_t app_pop_latest_input(void)
{
    __disable_irq();

    app_input_t event = app_peek_latest_input();
    if(event == APP_INPUT_NONE)
    {
        // nothing happened.
    }
    else if(event == APP_INPUT_SINGLE_TAP)
    {
        fifo_pop(&bma400_tap_queue, NULL); // clear the single tap event
    }
    else if(event == APP_INPUT_DOUBLE_TAP)
    {
        fifo_pop(&bma400_tap_queue, NULL); // clear the double tap event
        fifo_pop(&bma400_tap_queue, NULL); // as well as the single tap event
    }

    __enable_irq();
    return event;
}