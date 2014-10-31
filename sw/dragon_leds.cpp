#include <dragon_leds.h>
#include <dragon_pins.h>
#include <ac_printf.h>
#include <ac_util.h>
#include <Arduino.h>


static const bool DEBUG_LEDS = false;


int g_r;
int g_g;
int g_b;

void dragonLedInit()
{
  pinMode(dpin_eye_red, OUTPUT);
  pinMode(dpin_eye_green, OUTPUT);
  pinMode(dpin_eye_blue, OUTPUT);

  dragonLedColor(0,0,255);
}

void dragonLedColor(
  int r,
  int g,
  int b)
{
  g_r = acClamp(r, 0, 255);
  g_g = acClamp(g, 0, 255);
  g_b = acClamp(b, 0, 255);
  analogWrite(dpin_eye_red, g_r);
  analogWrite(dpin_eye_green, g_g);
  analogWrite(dpin_eye_blue, g_b);
  if (DEBUG_LEDS)
    acPrintf("Color: %d %d %d\n",g_r,g_g,g_b);
}

void dragonLedColorShift(
  int r,
  int g,
  int b,
  int duration_millis)
{
  dragonLedColor(r,g,b);
}

void dragonLedUpdate()
{
  dragonLedColorShift(10,10,10, 1000);
}

