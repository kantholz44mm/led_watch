#include "hal/time.h"
#include "cmsis/stm32l071xx.h"
#include "hal/gpio.h"

static volatile u64 seconds_elapsed_since_boot;

volatile bool on = false;

void isr_lptim1(void)
{
    LPTIM1->ICR = LPTIM1->ISR;
    seconds_elapsed_since_boot++;
}

void time_init(void)
{
    RCC->ICSCR &= ~RCC_ICSCR_MSIRANGE_Msk;  // clear clock configuration of MSI
    RCC->ICSCR |=  RCC_ICSCR_MSIRANGE_5;    // configure MSI to 2.097 MHz

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;      // enable PWR peripheral
    PWR->CR |= PWR_CR_DBP;                  // disable backup domain write protection. RCC_CSR is in that domain.

    if((RCC->CSR & RCC_CSR_LSEON) == 0U)
    {
        // enable LSE with all the drive its got.
        RCC->CSR |= RCC_CSR_LSEDRV_1 | RCC_CSR_LSEDRV_0;
        RCC->CSR |= RCC_CSR_LSEON;
        while(!(RCC->CSR & RCC_CSR_LSERDY));
    }

    PWR->CR &= ~PWR_CR_DBP;                 // enable backup domain write protection

    RCC->APB1ENR |= RCC_APB1ENR_LPTIM1EN;   // enable LPTIM1 clock
    LPTIM1->CR &= ~LPTIM_CR_ENABLE;         // disable LPTIM1 peripheral
    while((LPTIM1->CR & LPTIM_CR_ENABLE) != 0U);
    
    LPTIM1->CFGR &= ~LPTIM_CFGR_CKSEL;      // configure internal clocking
    LPTIM1->CFGR &= ~LPTIM_CFGR_COUNTMODE;  // configure internal clocking
    LPTIM1->CFGR &= ~LPTIM_CFGR_ENC;        // disable encoder mode

    LPTIM1->ICR = LPTIM_ICR_ARRMCF;         // clear interrupt if pending
    NVIC_SetPriority(LPTIM1_IRQn, 0);       // quite a high priority, but this is timing stuff.
    NVIC_EnableIRQ(LPTIM1_IRQn);            // enable LPTIM1 interrupt
    LPTIM1->IER  |= LPTIM_IER_ARRMIE;       // configure autoreload interrupt
    
    LPTIM1->CR |= LPTIM_CR_ENABLE;          // enable LPTIM1 peripheral
    while((LPTIM1->CR & LPTIM_CR_ENABLE) == 0U);
    
    
    RCC->CCIPR &= ~RCC_CCIPR_LPTIM1SEL;     // configure LSE as LPTIM1 source
    RCC->CCIPR |= RCC_CCIPR_LPTIM1SEL_0 | RCC_CCIPR_LPTIM1SEL_1; 
    LPTIM1->CR &= ~LPTIM_CFGR_TRIGEN;       // set to continuous mode
    LPTIM1->ARR = TIMER_FREQUENCY;          // autoreload after one second
    LPTIM1->CR |= LPTIM_CR_CNTSTRT;         // start the counter
}

timespan_t time_elapsed(void)
{
    u64 ticks = (u64)LPTIM1->CNT;
    u64 microseconds = TIMESPAN_FROM_SECONDS(ticks) / TIMER_FREQUENCY;
    return microseconds + TIMESPAN_FROM_SECONDS(seconds_elapsed_since_boot);
}

timespan_t time_elapsed_since(timespan_t start)
{
    timespan_t now = time_elapsed();
    if(start > now)
    {
        return 0;
    }
    else
    {
        return now - start;
    }
}

void time_wait_for(timespan_t duration)
{
    timespan_t until = time_elapsed() + duration;
    time_wait_until(until);
}

void time_wait_until(timespan_t timestamp)
{
    while(time_elapsed() < timestamp) /* do nothing */;
}