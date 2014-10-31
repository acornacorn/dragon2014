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
  if (duration_millis <= 0)
  {
    target_ = val_ = clamp(val);
    servo_.write(val_);
  }
  else
  {
    move_duration_millis_ = duration_millis;
    target_ = clamp(val);
    start_pos_ = val_;
    start_millis_ = millis();
  }
}

void AcServo::debugIncrement(int inc)
{
  val_ = acClamp(val_ + inc, 0, 255);
  acPrintf("Servo Value = %d\n", val_);
  target_ = val_;
  servo_.write(val_);
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
      val_ = nval;
      servo_.write(val_);
    }
  }
}

void AcServo::update()
{
  update(millis());
}

