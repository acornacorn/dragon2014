#ifndef AC_COUNTER_H
#define AC_COUNTER_H

#include <ac_timer.h>

// A counter that increments periodically.
// period_msec is how often it increments.
// Counts from 0 to wrap-1 then wraps to 0.
class AcCounter
{
public:
  void init(uint32_t period_msec, uint32_t wrap);

  // *val gets the current value.
  // returns true if it changed since last call.
  bool check(uint32_t* val);

private:
  AcTimer timer_;
  uint32_t val_;
  uint32_t wrap_;
};

#endif
