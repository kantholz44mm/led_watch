#include "hal/led.h"
#include "hal/i2c.h"
#include "hal/time.h"
#include "hal/gpio.h"
#include "hal/bma400.h"
#include "hal/rtc.h"
#include "animation.h"

u32 qbuf[4] = {0};
fifo_t q = {0};

int main(void)
{
    time_init();
    rtc_init();
    led_init();
    i2c_init();
    bma400_init();
    animation_init();

    rtc_datetime_t datetime = rtc_get();
    u8 hours = RTC_DATETIME_GET_HOURS(datetime);
    u8 minutes = RTC_DATETIME_GET_MINUTES(datetime);
    u8 seconds = RTC_DATETIME_GET_SECONDS(datetime);

    animation_push_show_time(datetime, true);
    
    while(true)
    {
        
    }
    
    return 0;
}
