#include "app/app_state_set_time.h"
#include "hal/bma400.h"
#include "hal/led.h"
#include "hal/rtc.h"
#include "animation.h"

#include <math.h>
#define PI 3.14159265358979323846

app_state_type_t private_tick(app_state_type_t type);




void app_state_set_time_hour_on_enter(void)
{
    animation_stop_and_clear();
}

void app_state_set_time_minute_on_enter(void)
{
    animation_stop_and_clear();
}



void app_state_set_time_hour_on_exit(void)
{
    while(animation_is_running());
}

void app_state_set_time_minute_on_exit(void)
{
    while(animation_is_running());
}



app_state_type_t app_state_set_time_hour_tick(void)
{
    return private_tick(APP_STATE_SET_TIME_HOUR);
}

app_state_type_t app_state_set_time_minute_tick(void)
{
    return private_tick(APP_STATE_SET_TIME_MINUTE);
}



app_state_type_t private_tick(app_state_type_t type)
{
    if(app_pop_latest_input() == APP_INPUT_DOUBLE_TAP)
    {
        switch(type)
        {
            case APP_STATE_SET_TIME_HOUR: 
            {
                animation_push_linear_run_direction(0U, 0U, 500U, true);
                animation_trigger_if_not_running();
                return APP_STATE_SHOW_TIME;
            }
            case APP_STATE_SET_TIME_MINUTE: 
            {
                animation_push_linear_run_direction(0U, 0U, 500U, true);
                animation_trigger_if_not_running();
                return APP_STATE_SET_TIME_HOUR;
            }
            default: 
            {
                return APP_STATE_SHOW_TIME;
            }
        }
    }

    static u8 current_led = 0U;
    static timespan_t current_led_since = 0U;
    static u8 time_components[2] = {0};

    float descent_angle = bma400_get_descent_angle();
    float normalized_descent_angle = (descent_angle / (2.0 * PI));
    u8 led = ((u8)(60.0f * normalized_descent_angle) + 15U) % 60U;

    if(led == current_led && time_elapsed_since(current_led_since) >= APP_TILT_ANGLE_LOCKIN_TIME)
    {
        switch(type)
        {
            case APP_STATE_SET_TIME_HOUR:
            {
                time_components[0] = led / 5;
                animation_push_blink(led, 100U, 100U, 600U);
                animation_trigger_if_not_running();
                current_led = 0U;
                current_led_since = time_elapsed();
                return APP_STATE_SET_TIME_MINUTE;
            }
            case APP_STATE_SET_TIME_MINUTE:
            {
                time_components[1] = led;
                animation_push_blink(led, 100U, 100U, 600U);
                animation_trigger_if_not_running();
                current_led = 0U;
                current_led_since = time_elapsed();

                rtc_datetime_t datetime = 0U
                    | RTC_DATETIME_FORMAT_HOURS(time_components[0])
                    | RTC_DATETIME_FORMAT_MINUTES(time_components[1])
                    | RTC_DATETIME_FORMAT_SECONDS(0U)
                    | RTC_DATETIME_FORMAT_YEARS(25U)
                    | RTC_DATETIME_FORMAT_MONTHS(10U)
                    | RTC_DATETIME_FORMAT_DAYS(25U);
                rtc_set(datetime);

                return APP_STATE_SHOW_TIME;
            }
            default:
            {
                return APP_STATE_SHOW_TIME;
            }
        }
    }
    else if(current_led != led)
    {
        current_led = led;
        current_led_since = time_elapsed();
    }

    led_set_active(led);

    return type;
}