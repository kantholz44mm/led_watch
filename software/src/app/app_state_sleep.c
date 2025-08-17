#include "app/app_state_sleep.h"
#include "hal/bma400.h"
#include "hal/standby.h"

void app_state_sleep_on_enter(void)
{
    bma400_set_input_filter(BMA400_TAPTYPE_DOUBLE);
    standby_enter();
}

void app_state_sleep_on_exit(void)
{
    // nothing
}

app_state_type_t app_state_sleep_tick(void)
{
    return APP_STATE_SHOW_TIME;
}