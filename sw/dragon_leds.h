#ifndef DRAGON_LEDS_H
#define DRAGON_LEDS_H

void dragonLedInit();
void dragonLedUpdate();

// set color immediately
void dragonLedColor(int r, int g, int b);

// transition to color over duration milliseconds
void dragonLedColorShift(int r, int g, int b, int duration_millis);


#endif
