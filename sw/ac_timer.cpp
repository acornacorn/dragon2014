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

void AcCounter::init(uint32_t period, uint32_t wrap)
{
  timer_.init(period);
  wrap_ = wrap;
}

bool AcCounter::check(uint32_t* val)
{
  bool rv = false;
  if (timer_.check())
  {
    val_++;
    if (val_ >= wrap_)
      val_ = 0;
    rv = true;
  }
  *val = val_;
  return rv;
}

