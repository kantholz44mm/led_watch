#pragma once
#include "types.h"

typedef struct {

    const char* app_name;
    const u8    app_version_major;
    const u8    app_version_minor;
    const u8    app_version_patch;

} meta_information_t;

extern const meta_information_t app_meta_information;