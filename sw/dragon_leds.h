#ifndef DRAGON_LEDS_H
#define DRAGON_LEDS_H

#include <ac_led_rgb.h>

void dragonLedInit();
void dragonLedUpdate();

// set color immediately
void dragonLedColorSet(int r, int g, int b);

// transition to color over duration milliseconds
void dragonLedColorShiftTo(int r, int g, int b, int duration_millis);

extern AcLedRgb g_eyes;


#endif
