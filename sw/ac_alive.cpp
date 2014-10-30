#include <ac_alive.h>
#include <ac_timer.h>
#include <ac_printf.h>
#include <dragon_pins.h>
#include <Arduino.h>

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
      acPrintf("alive %d\n", millis());
    }
    else
    {
      digitalWrite(dpin_alive_led, LOW);
    }
  }
}
