#include "app/app_state_show_time.h"
#include "hal/rtc.h"
#include "animation.h"
#include "hal/bma400.h"

void app_state_show_time_on_enter(void)
{
    rtc_datetime_t current_datetime = rtc_get();

    animation_stop_and_clear();
    animation_push_show_time(current_datetime, false);
    animation_trigger_if_not_running();
}

void app_state_show_time_on_exit(void)
{
    animation_stop_and_clear();
}

app_state_type_t app_state_show_time_tick(void)
{
    if(!animation_is_running())
    {
        return APP_STATE_SLEEP;
    }

    if(app_pop_latest_input() == APP_INPUT_DOUBLE_TAP)
    {
        return APP_STATE_SHOW_TEMP;
    }

    return APP_STATE_SHOW_TIME;
}