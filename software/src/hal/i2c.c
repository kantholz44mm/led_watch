#include "hal/i2c.h"
#include "cmsis/stm32l071xx.h"
#include "hal/gpio.h"

void i2c_init()
{   
    
    gpio_config_t io_config = {
        .mode = GPIO_MODE_ALTERNATE,
        .output_mode = GPIO_OUTPUT_MODE_OPENDRAIN,
        .output_speed = GPIO_OUTPUT_SPEED_VERY_HIGH,
        .pulltype = GPIO_PULLUP_NONE,
        .alternate_mode = 6
    };
    
    gpio_config(GPIO_PORT_A, 9, io_config);
    gpio_config(GPIO_PORT_A, 10, io_config);

    RCC->CCIPR    &= ~RCC_CCIPR_I2C1SEL;
    RCC->CCIPR    |=  RCC_CCIPR_I2C1SEL_0;
    RCC->APB1ENR  |=  RCC_APB1ENR_I2C1EN;
    RCC->APB1RSTR |=  RCC_APB1RSTR_I2C1RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

    I2C1->CR1 &= ~I2C_CR1_PE;
    while(I2C1->CR1 & I2C_CR1_PE);

    I2C1->TIMINGR |= 0x608U; // magic number generated with CubeMX tool

    I2C1->CR1 |= I2C_CR1_PE;
    while(!(I2C1->CR1 & I2C_CR1_PE));
}

void i2c_write(u8 address, const u8 *const bytes, u8 n)
{
    while(I2C1->CR2 & I2C_CR2_START);

    I2C1->CR2 &= ~I2C_CR2_SADD;
    I2C1->CR2 |=  address << 1;
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
    I2C1->CR2 &= ~I2C_CR2_NBYTES;
    I2C1->CR2 |=  ((u32)n) << I2C_CR2_NBYTES_Pos;

    I2C1->CR2 |=  I2C_CR2_START;

    u8 sent = 0;
    while((I2C1->ISR & I2C_ISR_TC) == 0)
    {
        if(I2C1->ISR & I2C_ISR_NACKF)
        {
            // error
            return;
        }
        
        if(I2C1->ISR & I2C_ISR_TXIS)
        {
            I2C1->TXDR = bytes[sent];
            sent++;
        }
    }

    I2C1->CR2 |= I2C_CR2_STOP;
}

void i2c_read(u8 address, u8* const bytes, u8 n)
{
    while(I2C1->CR2 & I2C_CR2_START);

    I2C1->CR2  &= ~I2C_CR2_SADD;
    I2C1->CR2  |=  address << 1U;
    I2C1->CR2  |=  I2C_CR2_RD_WRN;
    I2C1->CR2  &= ~I2C_CR2_NBYTES;
    I2C1->CR2  |=  ((u32)n) << I2C_CR2_NBYTES_Pos;
    I2C1->CR2  |=  I2C_CR2_START;

    u8 received = 0;
    while((I2C1->ISR & I2C_ISR_TC) == 0)
    {
        if(I2C1->ISR & I2C_ISR_RXNE)
        {
            bytes[received] = I2C1->RXDR;
            received++;
        }
    }

    I2C1->CR2 |= I2C_CR2_STOP;
}

void i2c_write_register(u8 slaveAddress, u8 registerAddress, u8 value)
{
    u8 buffer[2] = {registerAddress, value};
    i2c_write(slaveAddress, buffer, 2);
}

void i2c_read_register(u8 slaveAddress, u8 registerAddress, u8 size, u8* const bytes)
{
    i2c_write(slaveAddress, &registerAddress, 1);
    i2c_read(slaveAddress, bytes, size);
}