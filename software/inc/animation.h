#pragma once
#include "hal/led.h"
#include "fifo.h" 
#include "hal/rtc.h"

#define ANIMATION_QUEUE_SIZE 256U

typedef struct {
    
    u8 led;
    u16 duration;
    
} animation_frame_t;

extern fifo_t animation_queue;
extern animation_frame_t animation_queue_buffer[ANIMATION_QUEUE_SIZE];

void animation_init();
bool animation_add_frames(animation_frame_t* frames, u32 n);
void animation_push_linear_run(u8 from, u8 to, u16 total_duration);
void animation_push_blink(u8 led, u16 on_time, u16 off_time, u16 total_duration);
void animation_push_on(u8 led, u16 duration);
void animation_push_off(u16 duration);
void animation_push_show_time(rtc_datetime_t datetime, bool show_seconds);