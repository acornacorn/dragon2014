#include <ac_servo.h>
#include <ac_printf.h>

void AcServo::init(
      int pin,
      int init_val,
      int min_val,
      int max_val)
{
  min_val_ = min_val;
  max_val_ = max_val;
  val_ = clamp(init_val);
  target_ = val_;
  start_pos_ = val_;
  move_duration_millis_ = 1;
  start_millis_ = 0;
  servo_.attach(pin);
  servo_.write(val_);
}

void AcServo::go(
      int val, 
      int duration_millis)
{
acPrintf(" go %3d  val=%3d  t=%d\n",val, val_, millis());
  move_duration_millis_ = duration_millis;
  target_ = clamp(val);
  start_millis_ = millis();
}

void AcServo::update(uint32_t now_millis)
{
  if (target_ != val_)
  {
    int nval = target_;
    int dt = now_millis - start_millis_;
    if (dt < move_duration_millis_)
    {
      int delta_pos = target_ - start_pos_;
      nval = start_pos_ + ((dt * delta_pos) / move_duration_millis_);
      nval = clamp(nval);
    }
    if (nval != val_)
    {
acPrintf(" up %3d  val=%3d  t=%d\n",target_, nval, millis());
      val_ = nval;
      servo_.write(val_);
    }
  }
}

void AcServo::update()
{
  update(millis());
}

