#ifndef AC_SERVOS_H
#define AC_SERVOS_H

#include <Arduino.h>
#include <Servo.h>  
#include <ac_util.h>  

class AcServo
{
public:
  // Range of HS300 servo: d=29...176=s  mid: 107=l
  static const int DEF_MIN_VAL_ = 29;
  static const int DEF_MID_VAL_ = 107;
  static const int DEF_MAX_VAL_ = 176;

  void init(int pin,
            int init_val = DEF_MID_VAL_,
            int min_val = DEF_MIN_VAL_,
            int max_val = DEF_MAX_VAL_);

  void go(int val, int duration_millis);

  void update(uint32_t now_millis);
  void update();

private:
  int clamp(int val);

  int min_val_;
  int max_val_;
  int val_;

  int target_;
  int start_pos_;
  int start_millis_;
  int move_duration_millis_;

  Servo servo_;
};

inline int AcServo::clamp(int val)
{
  return acClamp(val, min_val_, max_val_);
}


#endif
