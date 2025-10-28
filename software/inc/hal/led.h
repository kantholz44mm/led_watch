#pragma once
#include "hal/gpio.h"
#include "cmsis/stm32l071xx.h"
#include <assert.h>
#include "types.h"

#define LED_SOURCE_PORT GPIO_PORT_B
#define LED_SINK_PORT   GPIO_PORT_A

#define LED_NUM_SOURCES         (15)
#define LED_NUM_SINKS           ( 4)
#define LED_SINK_SHIFT_WIDTH    (__builtin_ctz(LED_NUM_SINKS))
#define LED_SINK_MASK           (LED_NUM_SINKS - 1)
#define LED_NUM_LEDS            (60)

static_assert(__builtin_popcount(LED_NUM_SINKS) == 1);

#define LED_PIN_MASK_SINK   ((1UL << LED_NUM_SINKS) - 1)
#define LED_PIN_MASK_SOURCE ((1UL << LED_NUM_SOURCES) - 1)

void led_init(void);
void led_set_active(u8 led);
