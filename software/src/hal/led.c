#include "hal/led.h"

void led_init(void)
{
    gpio_config_t sink_config = {
        .mode = GPIO_MODE_OUTPUT,
        .output_mode = GPIO_OUTPUT_MODE_PUSHPULL,
        .output_speed = GPIO_OUTPUT_SPEED_LOW,
        .pulltype = GPIO_PULLUP_DOWN,
        .alternate_mode = 0,
    };

    gpio_config_t source_config = {
        .mode = GPIO_MODE_OUTPUT,
        .output_mode = GPIO_OUTPUT_MODE_PUSHPULL,
        .output_speed = GPIO_OUTPUT_SPEED_LOW,
        .pulltype = GPIO_PULLUP_NONE,
        .alternate_mode = 0,
    };

    for(u8 pin_sink = 0; pin_sink < LED_NUM_SINKS; pin_sink++)
    {
        gpio_config(LED_SINK_PORT, pin_sink, sink_config);
        gpio_write(LED_SINK_PORT, pin_sink, GPIO_HIGH);
    }

    for(u8 pin_source = 0; pin_source < LED_NUM_SOURCES; pin_source++)
    {
        gpio_config(LED_SOURCE_PORT, pin_source, source_config);
        gpio_write(LED_SOURCE_PORT, pin_source, GPIO_LOW);
    }
}

void led_set_active(u8 led)
{
    GPIO_TypeDef* const sink_base = (GPIO_TypeDef*)(IOPPERIPH_BASE + LED_SINK_PORT * 0x400UL);
    GPIO_TypeDef* const source_base = (GPIO_TypeDef*)(IOPPERIPH_BASE + LED_SOURCE_PORT * 0x400UL);

    if(led >= LED_NUM_LEDS)
    {
        led_deactivate();
        return;
    }

    led = (91 - led);
    if(led >= LED_NUM_LEDS)
    {
        led -= LED_NUM_LEDS;
    }

    const u8 source_pin = led >> LED_SINK_SHIFT_WIDTH;
    const u8 sink_pin = led & (u8)LED_SINK_MASK;

    sink_base->ODR |= LED_PIN_MASK_SINK;      // set all sinks inactive
    source_base->ODR &= ~LED_PIN_MASK_SOURCE; // set all sources inactive
    sink_base->ODR &= ~(1UL << sink_pin);     // set the single sink active
    source_base->ODR |= (1UL << source_pin);  // set the single source active
}

void led_deactivate(void)
{
    GPIO_TypeDef* const sink_base = (GPIO_TypeDef*)(IOPPERIPH_BASE + LED_SINK_PORT * 0x400UL);
    GPIO_TypeDef* const source_base = (GPIO_TypeDef*)(IOPPERIPH_BASE + LED_SOURCE_PORT * 0x400UL);

    sink_base->ODR |= LED_PIN_MASK_SINK;        // set all sinks inactive
    source_base->ODR &= ~LED_PIN_MASK_SOURCE;   // set all sources inactive
}