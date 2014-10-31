#include <dragon_leds.h>
#include <dragon_pins.h>
#include <dragon.h>
#include <ac_printf.h>
#include <ac_led_rgb.h>
#include <ac_counter.h>
#include <Arduino.h>

static const bool DEBUG_LEDS = false;

AcLedRgb g_eyes;

static AcCounter g_led_tst_cnt;

void dragonLedInit()
{
  g_eyes.init(dpin_eye_red, dpin_eye_green, dpin_eye_blue);

  g_led_tst_cnt.init(1000, 6);
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

  if (g_dragon.getMode() == Dragon::MODE_TEST)
  {
    uint32_t val;
    if (g_led_tst_cnt.check(&val))
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
}

