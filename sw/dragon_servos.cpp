#include <ac_servo.h>
#include <ac_counter.h>
#include <ac_printf.h>
#include <dragon_pins.h>
#include <dragon.h>

#if 0
static AcServo g_look_servo;
static AcServo g_leye_servo;
static AcServo g_reye_servo;
static AcServo g_lips_servo;

static AcCounter c1;
#endif

void dragonServoInit()
{
#if 0
  g_look_servo.init(dpin_look_servo, 107);  // look is pin 14
  g_leye_servo.init(dpin_leye_servo, 107);  // left eye is pin 15
  g_reye_servo.init(dpin_reye_servo, 107);  // right eye is pin 16
  g_lips_servo.init(dpin_lips_servo, 107);   // lip is pin 17

  g_dragon.setServos(g_look_servo,
                      g_leye_servo,
                      g_reye_servo,
                      g_lips_servo);

  c1.init(2000, 2);
#endif
}

void dragonServoUpdate()
{
#if 0
  uint32_t now_millis = millis();
  g_look_servo.update(now_millis);
  g_leye_servo.update(now_millis);
  g_reye_servo.update(now_millis);
  g_lips_servo.update(now_millis);

  uint32_t val;
  if (c1.check(&val))
  {
    if (val)
    {
      acPrintf("Down\n");
      g_look_servo.go(0, 1500);
    }
    else
    {
      acPrintf("Up\n");
      g_look_servo.go(255, 1500);
    }
  }
#endif
}
