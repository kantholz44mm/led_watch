#include "hal/led.h"
#include "hal/i2c.h"
#include "hal/time.h"
#include "hal/gpio.h"
#include "hal/bma400.h"

#include <string.h>
#include <math.h>
#define PI 3.14159265358979323846

bma400_accel_t acceleration;
float angle = 0.0;

volatile u32 num_taps = 0;
volatile u32 num_double_taps = 0;

void handle_single_tap(bma400_tap_type_t type)
{
    if(type & BMA400_TAPTYPE_SINGLE)
    {
        num_taps++;
    }
    if(type & BMA400_TAPTYPE_DOUBLE)
    {
        num_double_taps++;
    }
    return;
}

int main(void)
{
    time_init();
    led_init();
    i2c_init();
    bma400_init(handle_single_tap, handle_single_tap);

    while(true)
    {

    }

    #define N 1024
    float angles[N] = {0};

    while(true)
    {
        time_wait_for(TIMESPAN_FROM_MILLISECONDS(50));
        //bma400_read_acceleration(&acceleration);
        angle = (bma400_descent_angle() * 180.0 / PI);

        float cumul = 0.0;

        for(u32 i = 0; i < (N - 1); i++)
        {
            angles[i] = angles[i+1];
            cumul += angle;
        }

        angles[N - 1] = angle;
        angle = cumul / (float)N;

        u8 led = (u8)floorf((angle + 270.0) / (360.0 / 60.0)) % 60;
        led_set_active(led);
    }

    return 0;
}
