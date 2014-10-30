#ifndef AC_TIMER_H
#define AC_TIMER_H

#include <stdint.h>

class AcTimer
{
public:
  // (re)start the timer
  void init(uint32_t period);

  // true if it fired
  bool check(uint32_t now_millis);
  bool check();

private:
  uint32_t period_;
  uint32_t next_millis_;
};

class AcCounter
{
public:
  void init(uint32_t period, uint32_t wrap);
  bool check(uint32_t* val);
private:
  AcTimer timer_;
  uint32_t val_;
  uint32_t wrap_;
};

#endif
