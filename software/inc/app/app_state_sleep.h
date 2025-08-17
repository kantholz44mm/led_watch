#pragma once
#include "app/app.h"

void app_state_sleep_on_enter(void);
void app_state_sleep_on_exit(void);
app_state_type_t app_state_sleep_tick(void);