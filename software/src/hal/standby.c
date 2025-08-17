#include "hal/standby.h"
#include "cmsis/stm32l071xx.h"

void standby_enter(void)
{
    // clear last wakeup flag
    PWR->CR |= PWR_CR_CWUF;

    // enable wakeup pin 2
    PWR->CSR |= PWR_CSR_EWUP2;

    // configure power down deepsleep
    PWR->CR |= PWR_CR_PDDS;
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // wait for next interrupt (the only one on WKUP2 is the BMA400)
    __WFI();
}

bool standby_last_powerdown_was_standby(void)
{
    return (PWR->CSR & PWR_CSR_SBF) != 0U;
}