#include <ac_timer.h>
#include <Arduino.h>

void AcTimer::init(uint32_t period)
{
  period_ = period;
  next_millis_ = millis() + period_;
}

bool AcTimer::check(uint32_t now_millis)
{
  if ((int32_t)(now_millis - next_millis_) >= 0)
  {
    next_millis_ += period_;
    if ((int32_t)(now_millis - next_millis_) >= 0)
    {
      next_millis_ = now_millis + period_;
    }

#if 0
  Serial.println("timer fire #######################  A");
  Serial.flush();
  delay(10);
  yield();
#endif


    return true;
  }
  return false;
}

bool AcTimer::check()
{
  return check(millis());
}

void AcTimer::wait()
{
  while (!check())
  {
    delay(1);
  }
}

