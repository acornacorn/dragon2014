#include <ac_counter.h>
#include <Arduino.h>

void AcCounter::init(uint32_t period, uint32_t wrap)
{
  timer_.init(period);
  wrap_ = wrap;
  val_ = 0;
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

