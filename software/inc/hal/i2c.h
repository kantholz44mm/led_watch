#pragma once
#include "types.h"

void i2c_init();
void i2c_write(u8 address, const u8 *const bytes, u8 n);
void i2c_read(u8 address, u8* const bytes, u8 n);
void i2c_write_register(u8 slaveAddress, u8 registerAddress, u8 value);
void i2c_read_register(u8 slaveAddress, u8 registerAddress, u8 size, u8* const bytes);