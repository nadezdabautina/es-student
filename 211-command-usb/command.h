#pragma once

#include "pico/types.h" // uint


typedef void (*command_handler_t)(void);

struct command_t // описание (название, адрес функции)
{
    const char *name;
    command_handler_t handler;
};


extern const struct command_t commands[];
extern const uint command_count;