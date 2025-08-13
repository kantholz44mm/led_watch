#include "hal/rtc.h"
#include "cmsis/stm32l071xx.h"

const rtc_datetime_t RTC_DEFAULT_DATETIME = 0U
    | RTC_DATETIME_FORMAT_SECONDS( 0U)
    | RTC_DATETIME_FORMAT_MINUTES( 0U)
    | RTC_DATETIME_FORMAT_HOURS  (12U)
    | RTC_DATETIME_FORMAT_YEARS  (25U)
    | RTC_DATETIME_FORMAT_MONTHS ( 8U)
    | RTC_DATETIME_FORMAT_DAYS   (12U);

void rtc_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;      // enable PWR peripheral (if not already enabled)
    PWR->CR |= PWR_CR_DBP;                  // disable backup domain write protection. RCC_CSR is in that domain.

    if((RCC->CSR & RCC_CSR_LSEON) == 0U)
    {
        // enable LSE with all the drive its got.
        RCC->CSR |= RCC_CSR_LSEDRV_1 | RCC_CSR_LSEDRV_0;
        RCC->CSR |= RCC_CSR_LSEON;
        while(!(RCC->CSR & RCC_CSR_LSERDY));
    }

    if((RCC->CSR & RCC_CSR_RTCEN) == 0U)
    {
        RCC->CSR |= RCC_CSR_RTCSEL_LSE;
        RCC->CSR |= RCC_CSR_RTCEN;

        rtc_set(RTC_DEFAULT_DATETIME);
    }

    PWR->CR &= ~PWR_CR_DBP;                 // enable backup domain write protection
}

rtc_datetime_t rtc_get(void)
{
    return ((rtc_datetime_t)RTC->TR) | ((rtc_datetime_t)RTC->DR << 32U);
}

void rtc_set(rtc_datetime_t datetime)
{
    // disable write protection
    PWR->CR |= PWR_CR_DBP;
    RTC->WPR = 0xCAU;
    RTC->WPR = 0x53U;

    // start initialisation mode
    RTC->ISR |= RTC_ISR_INIT;
    while((RTC->ISR & RTC_ISR_INITF) == 0);

    // set time and date
    RTC->TR = (u32)((datetime >>  0U) & 0xFFFFFFFFU);
    RTC->DR = (u32)((datetime >> 32U) & 0xFFFFFFFFU);

    // disable initialisation mode
    RTC->ISR &= ~RTC_ISR_INIT;

    // enable write protection
    RTC->WPR = 0xCBU;
    RTC->WPR = 0x54U;
    PWR->CR &= ~PWR_CR_DBP;
}