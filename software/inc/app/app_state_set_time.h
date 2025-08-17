#pragma once
#include "app/app.h"

void app_state_set_time_hour_on_enter(void);
void app_state_set_time_minute_on_enter(void);
void app_state_set_time_second_on_enter(void);

void app_state_set_time_hour_on_exit(void);
void app_state_set_time_minute_on_exit(void);
void app_state_set_time_second_on_exit(void);

app_state_type_t app_state_set_time_hour_tick(void);
app_state_type_t app_state_set_time_minute_tick(void);
app_state_type_t app_state_set_time_second_tick(void);