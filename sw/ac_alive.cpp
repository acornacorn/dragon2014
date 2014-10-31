#include <ac_alive.h>
#include <ac_counter.h>
#include <ac_printf.h>
#include <dragon_pins.h>
#include <Arduino.h>

static const bool ALIVE_PRINTF = true;

static AcCounter g_alive_counter;

void acAliveInit()
{
  g_alive_counter.init(400, 2);
  pinMode(dpin_alive_led, OUTPUT);
}

void acAliveUpdate()
{
  uint32_t val;
  if (g_alive_counter.check(&val))
  {
    if (val)
    {
      digitalWrite(dpin_alive_led, HIGH);
      if (ALIVE_PRINTF)
        acPrintf("alive %d\n", millis());
    }
    else
    {
      digitalWrite(dpin_alive_led, LOW);
    }
  }
}

