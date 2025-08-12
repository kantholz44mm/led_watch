#pragma once
#include "types.h"
#include "cmsis/stm32l071xx.h"

#define GPIO_LOW   0
#define GPIO_HIGH  1

typedef enum {

    GPIO_PORT_A = 0,
    GPIO_PORT_B = 1,
    GPIO_PORT_C = 2,
    GPIO_PORT_D = 3,
    GPIO_PORT_E = 4,
    GPIO_PORT_H = 7

} gpio_port_t;

typedef enum {

    GPIO_MODE_INPUT     = 0b00,
    GPIO_MODE_OUTPUT    = 0b01,
    GPIO_MODE_ALTERNATE = 0b10,
    GPIO_MODE_ANALOG    = 0b11

} gpio_mode_t;


typedef enum {

    GPIO_OUTPUT_MODE_PUSHPULL  = 0b0,
    GPIO_OUTPUT_MODE_OPENDRAIN = 0b1

} gpio_output_mode_t;


typedef enum {

    GPIO_OUTPUT_SPEED_LOW       = 0b00,
    GPIO_OUTPUT_SPEED_MEDIUM    = 0b01,
    GPIO_OUTPUT_SPEED_HIGH      = 0b10,
    GPIO_OUTPUT_SPEED_VERY_HIGH = 0b11,

} gpio_output_speed_t;


typedef enum {

    GPIO_PULLUP_NONE = 0b00,
    GPIO_PULLUP_UP   = 0b01,
    GPIO_PULLUP_DOWN = 0b10,

} gpio_pulltype_t;



typedef struct {

    gpio_mode_t         mode;
    gpio_output_mode_t  output_mode;
    gpio_output_speed_t output_speed;
    gpio_pulltype_t     pulltype;
    u8                  alternate_mode;

} gpio_config_t;

void gpio_config(gpio_port_t port, u8 pin, gpio_config_t config);
u8 gpio_read(gpio_port_t port, u8 pin);
void gpio_write(gpio_port_t port, u8 pin, u8 value);