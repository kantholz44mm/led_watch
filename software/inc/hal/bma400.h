#pragma once
#include "types.h"
#include "fifo.h"
#include "hal/time.h"

#define BMA400_SLAVE_ADDRESS            0x14U
#define BMA400_TAP_EVENT_QUEUE_SIZE     8U

typedef enum {

    BMA400_REGISTER_CHIPID          = 0x00U,
    BMA400_REGISTER_ERRREG          = 0x02U,
    BMA400_REGISTER_STATUS          = 0x03U,
    BMA400_REGISTER_ACC_X_LSB       = 0x04U,
    BMA400_REGISTER_ACC_X_MSB       = 0x05U,
    BMA400_REGISTER_ACC_Y_LSB       = 0x06U,
    BMA400_REGISTER_ACC_Y_MSB       = 0x07U,
    BMA400_REGISTER_ACC_Z_LSB       = 0x08U,
    BMA400_REGISTER_ACC_Z_MSB       = 0x09U,
    BMA400_REGISTER_TEMP_DATA       = 0x11U,
    BMA400_REGISTER_INT_STAT0       = 0x0EU,
    BMA400_REGISTER_INT_STAT1       = 0x0FU,
    BMA400_REGISTER_INT_STAT2       = 0x10U,
    BMA400_REGISTER_ACC_CONFIG0     = 0x19U,
    BMA400_REGISTER_ACC_CONFIG1     = 0x1AU,
    BMA400_REGISTER_ACC_CONFIG2     = 0x1BU,
    BMA400_REGISTER_INT_CONFIG0     = 0x1FU,
    BMA400_REGISTER_INT_CONFIG1     = 0x20U,
    BMA400_REGISTER_INT1_MAP        = 0x21U,
    BMA400_REGISTER_INT2_MAP        = 0x22U,
    BMA400_REGISTER_INT12_MAP       = 0x23U,
    BMA400_REGISTER_INT12_IO_CTRL   = 0x24U,
    BMA400_REGISTER_TAP_CONFIG      = 0x57U,
    BMA400_REGISTER_TAP_CONFIG1     = 0x58U,
    BMA400_REGISTER_CMD             = 0x7EU,

} bma400_register_t;

typedef struct {

    i16 x;
    i16 y;
    i16 z;

} bma400_accel_t;

typedef enum {

    BMA400_TAPTYPE_SINGLE = 0x01,
    BMA400_TAPTYPE_DOUBLE = 0x02,

} bma400_tap_type_t;

typedef struct {

    bma400_tap_type_t type;
    timespan_t timestamp;

} bma400_tap_event_t;

extern fifo_t bma400_tap_queue;

void bma400_init(void);
void bma400_set_input_filter(bma400_tap_type_t type);
void bma400_get_acceleration(bma400_accel_t* accel);
float bma400_get_descent_angle(void);
i8 bma400_get_temperature(void);