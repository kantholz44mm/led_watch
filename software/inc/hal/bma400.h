#pragma once
#include "types.h"

#define BMA400_SLAVE_ADDRESS    0x14U

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
    BMA400_TAPTYPE_BOTH   = 0x03,

} bma400_tap_type_t;

typedef void(*bma400_tap_handler_t)(bma400_tap_type_t);

void bma400_init(bma400_tap_handler_t single_tap, bma400_tap_handler_t double_tap);
void bma400_read_acceleration(bma400_accel_t* accel);
float bma400_descent_angle(void);