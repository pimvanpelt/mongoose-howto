#pragma once

#include "mgos.h"

void button_handler(int gpio, void *user_data);
bool led_getstate(void);
void button_led_init();
