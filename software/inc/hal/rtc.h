#pragma once
#include "types.h"

typedef u64 rtc_datetime_t;

#define RTC_DATETIME_GET_SECONDS(t)      (u8)((((t >>  4U) & 0x7U) * 10U) + ((t >>  0U) & 0xFU))
#define RTC_DATETIME_GET_MINUTES(t)      (u8)((((t >> 12U) & 0x7U) * 10U) + ((t >>  8U) & 0xFU))
#define RTC_DATETIME_GET_HOURS(t)        (u8)((((t >> 20U) & 0x3U) * 10U) + ((t >> 16U) & 0xFU))

#define RTC_DATETIME_GET_DAYS(d)         (u8)((((d >> 36U) & 0x3U) * 10U) + ((d >> 32U) & 0xFU))
#define RTC_DATETIME_GET_MONTHS(d)       (u8)((((d >> 44U) & 0x1U) * 10U) + ((d >> 40U) & 0xFU))
#define RTC_DATETIME_GET_YEARS(d)       (u16)((((d >> 52U) & 0xFU) * 10U) + ((d >> 48U) & 0xFU))

#define RTC_DATETIME_FORMAT_SECONDS(s)  ((rtc_datetime_t)((s / 10) & 0x7U) <<  4U) | ((rtc_datetime_t)((s % 10) & 0xFU) <<  0U)
#define RTC_DATETIME_FORMAT_MINUTES(m)  ((rtc_datetime_t)((m / 10) & 0x7U) << 12U) | ((rtc_datetime_t)((m % 10) & 0xFU) <<  8U)
#define RTC_DATETIME_FORMAT_HOURS(h)    ((rtc_datetime_t)((h / 10) & 0x3U) << 20U) | ((rtc_datetime_t)((h % 10) & 0xFU) << 16U)

#define RTC_DATETIME_FORMAT_DAYS(d)     ((rtc_datetime_t)((d / 10) & 0x3U) << 36U) | ((rtc_datetime_t)((d % 10) & 0xFU) << 32U)
#define RTC_DATETIME_FORMAT_MONTHS(m)   ((rtc_datetime_t)((m / 10) & 0x1U) << 44U) | ((rtc_datetime_t)((m % 10) & 0xFU) << 40U)
#define RTC_DATETIME_FORMAT_YEARS(y)    ((rtc_datetime_t)((y / 10) & 0xFU) << 52U) | ((rtc_datetime_t)((y % 10) & 0xFU) << 48U)

extern const rtc_datetime_t RTC_DEFAULT_DATETIME;

void rtc_init(void);
rtc_datetime_t rtc_get(void);
void rtc_set(rtc_datetime_t datetime);
