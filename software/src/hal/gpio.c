#include "hal/gpio.h"

void gpio_config(gpio_port_t port, u8 pin, gpio_config_t config)
{
    GPIO_TypeDef* base = (GPIO_TypeDef*)(IOPPERIPH_BASE + port * 0x400UL);

    // enable peripheral
    RCC->IOPENR |= (1UL << port);
    
    // set mode
    base->MODER &= ~(0b11UL << (pin * 2));
    base->MODER |= ((u32)config.mode << (pin * 2));

    // set output type
    base->OTYPER &= ~(0b1UL << pin);
    base->OTYPER |= ((u32)config.output_mode << pin);

    // set output speed
    base->OSPEEDR &= ~(0b11UL << (pin * 2));
    base->OSPEEDR |= ((u32)config.output_speed << (pin * 2));

    // set pullup/pulldown
    base->PUPDR &= ~(0b11UL << (pin * 2));
    base->PUPDR |= ((u32)config.pulltype << (pin * 2));

    if(config.mode == GPIO_MODE_ALTERNATE)
    {
        u8 afr_bank = (pin / 8UL);
        u8 afr_offset = (pin % 8UL) * 4UL;
        base->AFR[afr_bank] &= ~((0b1111UL) << afr_offset);
        base->AFR[afr_bank] |= (((u32)config.alternate_mode % 8UL) << afr_offset);
    }
}

u8 gpio_read(gpio_port_t port, u8 pin)
{
    GPIO_TypeDef* base = (GPIO_TypeDef*)(IOPPERIPH_BASE + port * 0x400UL);
    return (u8)((base->IDR >> pin) & 1UL);
}

void gpio_write(gpio_port_t port, u8 pin, u8 value)
{
    GPIO_TypeDef* base = (GPIO_TypeDef*)(IOPPERIPH_BASE + port * 0x400UL);
    base->BSRR = (1UL << (pin + (!value * 16U)));
}