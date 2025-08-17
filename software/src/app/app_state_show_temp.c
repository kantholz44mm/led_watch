#include "app/app_state_show_temp.h"
#include "hal/bma400.h"
#include "animation.h"

void app_state_show_temp_on_enter(void)
{
    bma400_set_input_filter(BMA400_TAPTYPE_DOUBLE);
    i8 temperature = bma400_get_temperature();
    
    animation_stop_and_clear();
    animation_push_show_temperature(temperature);
    animation_trigger_if_not_running();
}

void app_state_show_temp_on_exit(void)
{
    animation_stop_and_clear();
}

app_state_type_t app_state_show_temp_tick(void)
{
    if(!animation_is_running())
    {
        return APP_STATE_SLEEP;
    }

    if(app_pop_latest_input() == APP_INPUT_DOUBLE_TAP)
    {
        return APP_STATE_SET_TIME_HOUR;
    }

    return APP_STATE_SHOW_TEMP;
}