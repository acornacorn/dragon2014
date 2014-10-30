#include <ac_alive.h>
#include <ac_timer.h>
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
#if 0
Serial.println("Alive xx");
Serial.flush();
delay(10);

char buf[100];
int m = millis();
snprintf(buf, sizeof(buf), "millis=%d sz(i)=%d", m, (int)sizeof(int));
Serial.println(buf);
Serial.flush();
delay(10);
#endif


  uint32_t val;
  if (g_alive_counter.check(&val))
  {
    if (val)
    {
      digitalWrite(dpin_alive_led, HIGH);
    }
    else
    {
      digitalWrite(dpin_alive_led, LOW);
    }
  }
}

