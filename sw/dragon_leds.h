#ifndef DRAGON_LEDS_H
#define DRAGON_LEDS_H

void dragonLedInit();
void dragonLedUpdate();

// set color immediately
void dragonLedColorSet(int r, int g, int b);

// transition to color over duration milliseconds
void dragonLedColorShiftTo(int r, int g, int b, int duration_millis);


#endif
