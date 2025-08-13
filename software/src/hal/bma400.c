#include "hal/bma400.h"
#include "hal/i2c.h"
#include <math.h>
#include "cmsis/stm32l071xx.h"
#include "hal/gpio.h"

#define PI 3.14159265358979323846

void isr_exti_15_4(void)
{
    // get status, which resets the pending register on the BMA400
    u8 status_byte;
    i2c_read_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_INT_STAT1, 1, &status_byte);
    // acknowledge interrupt
    EXTI->PR |= EXTI_PR_PIF13;

    if(status_byte & 0x8U)
    {
        // double tap
    }
    else if(status_byte & 0x4U)
    {
        // single tap
    }

}

void bma400_init(void)
{
    // perform soft reset
    i2c_write_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_CMD, 0xB6U);

    // set to "normal" mode
    i2c_write_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_ACC_CONFIG0, 0x02U);

    // set range to +/-2g, sample rate to 200Hz, high accuracy oversampling
    i2c_write_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_ACC_CONFIG1, 0x39U);

    gpio_config_t int_pin_cfg = {
        .mode = GPIO_MODE_INPUT,
        .output_mode = GPIO_OUTPUT_MODE_OPENDRAIN,
        .output_speed = GPIO_OUTPUT_SPEED_LOW,
        .pulltype = GPIO_PULLUP_UP,
        .alternate_mode = 0
    };

    gpio_config(GPIO_PORT_C, 13, int_pin_cfg);

    // enable SYSCFG and wait for it to boot
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    while((RCC->APB2ENR & RCC_APB2ENR_SYSCFGEN) == 0U);

    // set port C as input for EXTI13
    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
    SYSCFG->EXTICR[3] |=  SYSCFG_EXTICR4_EXTI13_PC;

    // disable rising, enable falling edge on EXTI13
    EXTI->RTSR &= ~EXTI_RTSR_RT13;
    EXTI->FTSR |=  EXTI_FTSR_FT13;
    // clear any pending interrupt on EXTI13
    EXTI->PR   |=  EXTI_PR_PIF13;
    // enable EXTI 13 interrupt by unmasking
    EXTI->IMR  |=  EXTI_IMR_IM13;

    // finally, enable in NVIC
    NVIC_SetPriority(EXTI4_15_IRQn, 1);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

    // set tap configuration: x axis, moderate sensitivity
    i2c_write_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_TAP_CONFIG, 0x14U);

    // map tap / double tap interrupt to INT1 pin
    i2c_write_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_INT12_MAP, 0x04U);

    // set INT1 pin to active low, open drain drive
    i2c_write_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_INT12_IO_CTRL, 0x04U);

    // enable tap / double tap interrupt and configure interrupt latching
    i2c_write_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_INT_CONFIG1, 0x8CU);
}

void bma400_read_acceleration(bma400_accel_t* accel)
{
    i2c_read_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_ACC_X_LSB, 2, (u8*)&accel->x);
    i2c_read_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_ACC_Y_LSB, 2, (u8*)&accel->y);
    i2c_read_register(BMA400_SLAVE_ADDRESS, BMA400_REGISTER_ACC_Z_LSB, 2, (u8*)&accel->z);

    // convert to signed integers
    if(accel->x > 0x7FF) { accel->x -= 0x1000; }
    if(accel->y > 0x7FF) { accel->y -= 0x1000; }
    if(accel->z > 0x7FF) { accel->z -= 0x1000; }
}

float bma400_descent_angle(void)
{
    static bma400_accel_t accel = {0};
    bma400_read_acceleration(&accel);
    float angle = atan2f((float)accel.y, (float)accel.x);
    return angle < 0.0 ? angle + 2.0 * PI : angle;
}
