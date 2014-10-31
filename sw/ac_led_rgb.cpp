#include <ac_led_rgb.h>
#include <ac_util.h>
#include <ac_printf.h>
#include <Arduino.h>

void AcLedRgb::init(
      int rpin, 
      int gpin, 
      int bpin)
{
  pin_[R] = rpin;
  pin_[G] = gpin;
  pin_[B] = bpin;

  pinMode(rpin, OUTPUT);
  pinMode(gpin, OUTPUT);
  pinMode(bpin, OUTPUT);

  set(0,0,0);
}

void AcLedRgb::write()
{
  analogWrite(pin_[R], val_[R]);
  analogWrite(pin_[G], val_[G]);
  analogWrite(pin_[B], val_[B]);
}

void AcLedRgb::set(
      int r, 
      int g, 
      int b)
{
  val_[R] = acClamp(r, 0, 255);
  val_[G] = acClamp(g, 0, 255);
  val_[B] = acClamp(b, 0, 255);
  shifting_ = false;
  write();
}

void AcLedRgb::shiftTo(
      int r, 
      int g, 
      int b, 
      int duration_millis)
{
  duration_millis_ = duration_millis;
  target_[R] = acClamp(r, 0, 255);
  target_[G] = acClamp(g, 0, 255);
  target_[B] = acClamp(b, 0, 255);

  start_[R] = val_[R];
  start_[G] = val_[G];
  start_[B] = val_[B];

  start_millis_ = millis();

  shifting_ = true;
}

void AcLedRgb::update()
{
  if (shifting_)
  {
    int dt = millis() - start_millis_;

    if (dt >= duration_millis_)
    {
      val_[R] = target_[R];
      val_[G] = target_[G];
      val_[B] = target_[B];
      shifting_ = false;
      return;
    }

    bool changed = false;

    for (int i = 0 ; i < CHANNEL_CNT ; ++i)
    {
      int delta_val = target_[i] - start_[i];
      int nval = start_[i] + ((dt * delta_val) / duration_millis_);
      nval = acClamp(nval, 0, 255);

      if (nval != val_[i])
      {
        val_[i] = nval;
        changed = true;
      }
    }

    if (changed)
      write();
  }
}

void AcLedRgb::debugIncrement(
      int chan,
      int inc)
{
  if ((uint32_t)chan < 3)
  {
    val_[chan] = acClamp(val_[chan] + inc, 0, 255);
    acPrintf("Value = %d\n", (int)val_[chan]);
    set(val_[R], val_[G], val_[B]);
  }
}

