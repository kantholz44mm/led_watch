#include "types.h"

extern void _estack(void);
extern void _reset(void);

#define EVER ;;
#define DECLARE_ISR(name) void __attribute__((weak, alias("default_interrupt_handler"))) name(void)

void default_interrupt_handler(void)
{
    for(EVER) {
        __asm("nop");
    }
}

DECLARE_ISR(isr_nmi);
DECLARE_ISR(isr_hard_fault);
DECLARE_ISR(isr_svc);
DECLARE_ISR(isr_pendsv);
DECLARE_ISR(isr_systick);
DECLARE_ISR(isr_watchdog);
DECLARE_ISR(isr_pvd);
DECLARE_ISR(isr_rtc);
DECLARE_ISR(isr_flash);
DECLARE_ISR(isr_rcc_crs);
DECLARE_ISR(isr_exti_1_0);
DECLARE_ISR(isr_exti_3_2);
DECLARE_ISR(isr_exti_15_4);
DECLARE_ISR(isr_dma1_channel_1);
DECLARE_ISR(isr_dma1_channel_3_2);
DECLARE_ISR(isr_dma1_channel_7_4);
DECLARE_ISR(isr_adc_comp);
DECLARE_ISR(isr_lptim1);
DECLARE_ISR(isr_usart4_5);
DECLARE_ISR(isr_tim2);
DECLARE_ISR(isr_tim3);
DECLARE_ISR(isr_tim6);
DECLARE_ISR(isr_tim7);
DECLARE_ISR(isr_21);
DECLARE_ISR(isr_i2c3);
DECLARE_ISR(isr_tim22);
DECLARE_ISR(isr_i2c1);
DECLARE_ISR(isr_i2c2);
DECLARE_ISR(isr_spi1);
DECLARE_ISR(isr_spi2);
DECLARE_ISR(isr_usart1);
DECLARE_ISR(isr_usart2);
DECLARE_ISR(isr_lpuart1_aes);


__attribute__((section(".vectors"))) void (*const tab[16 + 32])(void) = {
    // 0
    _estack,
    _reset,
    isr_nmi,
    isr_hard_fault,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    isr_svc,
    NULL,
    NULL,
    isr_pendsv,
    isr_systick,
    // 16
    isr_watchdog,
    isr_pvd,
    isr_rtc,
    isr_flash,
    isr_rcc_crs,
    isr_exti_1_0,
    isr_exti_3_2,
    isr_exti_15_4,
    NULL,
    isr_dma1_channel_1,
    isr_dma1_channel_3_2,
    isr_dma1_channel_7_4,
    isr_adc_comp,
    isr_lptim1,
    isr_usart4_5,
    isr_tim2,
    // 32
    isr_tim3,
    isr_tim6,
    isr_tim7,
    NULL,
    isr_21,
    isr_i2c3,
    isr_tim22,
    isr_i2c1,
    isr_i2c2,
    isr_spi1,
    isr_spi2,
    isr_usart1,
    isr_usart2,
    isr_lpuart1_aes,
    NULL,
    NULL,
};