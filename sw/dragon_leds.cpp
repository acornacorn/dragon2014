#include <dragon_leds.h>
#include <dragon_pins.h>
#include <ac_printf.h>
#include <ac_led_rgb.h>
#include <ac_counter.h>
#include <Arduino.h>

static const bool DEBUG_LEDS = true;

static AcLedRgb g_eyes;
static AcCounter c1;

void dragonLedInit()
{
  g_eyes.init(dpin_eye_red, dpin_eye_green, dpin_eye_blue);

  dragonLedColorSet(0,0,255);
  dragonLedColorShiftTo(255,10,10, 1000);

  c1.init(1000, 6);
}

void dragonLedColorSet(
  int r,
  int g,
  int b)
{
  g_eyes.set(r,g,b);

  if (DEBUG_LEDS)
    acPrintf("Color: %d %d %d\n",r,g,b);
}

void dragonLedColorShiftTo(
  int r,
  int g,
  int b,
  int duration_millis)
{
  g_eyes.shiftTo(r,g,b,duration_millis);

  if (DEBUG_LEDS)
  {
    acPrintf("Color: shift to %d %d %d  dur=%d\n",
      r,g,b,duration_millis);
  }
}


void dragonLedUpdate()
{
  g_eyes.update();

  uint32_t val;
  if (c1.check(&val))
  {
    switch(val)
    {
    case 0: dragonLedColorShiftTo(255,0,0,500); break;
    case 1: dragonLedColorShiftTo(0,255,0,500); break;
    case 2: dragonLedColorShiftTo(0,0,255,500); break;
    case 3: dragonLedColorShiftTo(255,255,0,500); break;
    case 4: dragonLedColorShiftTo(255,0,255,500); break;
    case 5: dragonLedColorShiftTo(0,255,255,500); break;
    }
  }
}

