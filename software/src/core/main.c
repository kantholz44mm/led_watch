#include "hal/led.h"
#include "hal/i2c.h"
#include "hal/time.h"
#include "hal/gpio.h"
#include "hal/bma400.h"
#include "hal/rtc.h"
#include "animation.h"
#include "app/app.h"

int main(void)
{
    // initialize every peripheral needed
    time_init();
    rtc_init();
    led_init();
    i2c_init();
    bma400_init();
    animation_init();

    // the workhorse
    app_run();

    return 0;
}
