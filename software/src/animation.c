#include "animation.h"
#include "util.h"

fifo_t animation_queue = {0};
animation_frame_t animation_queue_buffer[ANIMATION_QUEUE_SIZE] = {0};

void animation_trigger_if_not_running(void)
{
    bool animation_running = (TIM6->CR1 & TIM_CR1_CEN) != 0U;
    if(!animation_running)
    {
        TIM6->EGR |= TIM_EGR_UG;    // trigger first interrupt
    }
}

bool animation_is_running(void)
{
    return !fifo_is_empty(&animation_queue) || ((TIM6->CR1 & TIM_CR1_CEN) != 0U);
}

void isr_tim6(void)
{
    TIM6->SR &= ~TIM_SR_UIF;                  // acknowledge interrupt

    animation_frame_t frame;
    bool frame_available = fifo_pop(&animation_queue, (u8*)&frame);
    
    if(frame_available)
    {
        led_set_active(frame.led);                // activate next LED for current frame
        TIM6->ARR = MAX(frame.duration, 5U);      // set timer limit
        TIM6->CR1 |= TIM_CR1_CEN;                 // enable timer for next frame
    }
    else
    {
        led_set_active(LED_NUM_LEDS);
    }
}

void animation_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;  // enable animation timer peripheral clock
    TIM6->CR1  &= ~TIM_CR1_URS;          // enable software generated interrupts
    TIM6->CR1  |= TIM_CR1_OPM;           // set one pulse mode -> interrupt routine might reenable.
    TIM6->DIER |= TIM_DIER_UIE;          // enable overflow interrupt
    TIM6->PSC = 2097;                    // set prescaler (APB1 clock = MSI @ 2.097MHz, therefore PRESC = 2097 -> 1kHz)

    NVIC_SetPriority(TIM6_IRQn, 2);
    NVIC_EnableIRQ(TIM6_IRQn);

    fifo_init(&animation_queue, (u8*)&animation_queue_buffer, sizeof(animation_frame_t), ANIMATION_QUEUE_SIZE);
}

void animation_stop_and_clear(void)
{
    // stop timer
    TIM6->CR1 &= ~TIM_CR1_CEN;

    // delete remaining animation frames
    fifo_clear(&animation_queue);
}

bool animation_add_frames(animation_frame_t* frames, u32 n)
{
    if(fifo_num_free(&animation_queue) < n)
    {
        return false;
    }

    if(n == 0U)
    {
        return true;
    }

    for(u32 i = 0; i < n; i++)
    {
        fifo_push(&animation_queue, (u8*)&frames[i], false);
    }

    return true;
}

void animation_push_linear_run(u8 from, u8 to, u16 total_duration)
{   
    u8 cw  = (to + 60U - from) % 60U;
    u8 ccw = (from + 60U - to) % 60U;
    bool run_clockwise = cw <= ccw;

    if(cw == 0U && ccw == 0U)
    {
        // do full circle run if same LED is given twice.
        cw = 59U;
    }

    i32 step = -1 + ((i8)run_clockwise * 2);
    i32 num_leds = (i32)(run_clockwise ? cw : ccw) + 1U;
    u16 duration_per_led = total_duration / (u16)num_leds;

    animation_frame_t frame = {
        .led = from,
        .duration = duration_per_led
    };

    for(i32 i = 0; i < num_leds; i++)
    {
        u8 led = (60 + (i32)from + step * i) % 60;
        frame.led = led;
        fifo_push(&animation_queue, (u8*)&frame, false);
    }
}

void animation_push_linear_run_direction(u8 from, u8 to, u16 total_duration, bool clockwise)
{
    u8 distance = clockwise ?
        ((to + 60U - from) % 60U) :
        ((from + 60U - to) % 60U);

    if(distance == 0U)
    {
        // do full circle run if same LED is given twice.
        distance = 59U;
    }

    i32 step = -1 + ((i8)clockwise * 2);
    i32 num_leds = distance + 1U;
    u16 duration_per_led = total_duration / (u16)num_leds;

    animation_frame_t frame = {
        .led = from,
        .duration = duration_per_led
    };

    for(i32 i = 0; i < num_leds; i++)
    {
        u8 led = (60 + (i32)from + step * i) % 60;
        frame.led = led;
        fifo_push(&animation_queue, (u8*)&frame, false);
    }

    animation_push_off(10U);
}

void animation_push_blink(u8 led, u16 on_time, u16 off_time, u16 total_duration)
{
    if(led >= 60U)
    {
        return;
    }

    u32 period = on_time + off_time;
    u32 repetitions = total_duration / period;

    animation_frame_t frames[2] = {
        {
            .led = led,
            .duration = on_time
        },
        {
            .led = LED_NUM_LEDS,
            .duration = off_time
        }
    };

    for(u32 i = 0; i < repetitions; i++)
    {
        fifo_push(&animation_queue, (u8*)&frames[0], false);
        fifo_push(&animation_queue, (u8*)&frames[1], false);
    }
}

void animation_push_on(u8 led, u16 duration)
{
    animation_frame_t frame = {
        .led = led,
        .duration = duration
    };

    fifo_push(&animation_queue, (u8*)&frame, false);
}

void animation_push_off(u16 duration)
{
    animation_frame_t frame = {
        .led = LED_NUM_LEDS,
        .duration = duration
    };

    fifo_push(&animation_queue, (u8*)&frame, false);
}

void animation_push_run_to_leds(u8* led, u8 n)
{
    u8 current_led = 0U;

    for(u32 i = 0; i < n; i++)
    {
        animation_push_linear_run(current_led, led[i], 500U);
        animation_push_off(50U);
        animation_push_blink(led[i], 100U, 100U, 600U);
        animation_push_on(led[i], 250U);

        current_led = led[i];
    }

    animation_push_off(10U);
}

void animation_push_show_time(rtc_datetime_t datetime, bool show_seconds)
{
    u8 led_hours_minutes_seconds[3] = {
        (RTC_DATETIME_GET_HOURS(datetime) % 12U) * 5U,
        (RTC_DATETIME_GET_MINUTES(datetime)),
        (RTC_DATETIME_GET_SECONDS(datetime))
    };
    
    u8 num_leds = 2U + show_seconds;
    animation_push_run_to_leds(led_hours_minutes_seconds, num_leds);
}

void animation_push_show_temperature(i8 temperature)
{
    bool clockwise = temperature >= 0;
    u8 target_led = (u8)((60 + temperature) % 60);

    animation_push_linear_run_direction(0U, target_led, 1000U, clockwise);
    animation_push_off(50U);
    animation_push_blink(target_led, 100U, 100U, 600U);
    animation_push_on(target_led, 500U);
    animation_push_off(10U);
}