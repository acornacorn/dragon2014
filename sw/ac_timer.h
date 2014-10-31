#ifndef AC_TIMER_H
#define AC_TIMER_H

#include <stdint.h>

// a repeating timer.
class AcTimer
{
public:
  // (re)start the timer
  void init(uint32_t period);

  // true if it fired since last call.
  // This also resets the timer for the next period.
  bool check(uint32_t now_millis);
  bool check();

  // wait for it to fire
  void wait();

private:
  uint32_t period_;
  uint32_t next_millis_;
};

#endif
