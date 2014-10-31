#ifndef AC_LED_RGB_H
#define AC_LED_RGB_H

#include <stdint.h>

class AcLedRgb
{
public:
  void init(int rpin, int gpin, int bpin);
  void set(int r, int g, int b);
  void shiftTo(int r, int g, int b, int duration_millis);
  void update();

private:
  void write();

  enum {
    R,G,B, CHANNEL_CNT
  };

  uint8_t pin_[CHANNEL_CNT];   // pin for each color component

  uint8_t val_[CHANNEL_CNT];    // current value

  int target_[CHANNEL_CNT];     // target of shift
  int start_[CHANNEL_CNT];      // val at start of shift
  int start_millis_;            // time at start of shift
  int duration_millis_;         // how long to take to reach target color
  bool shifting_;               // true if shifting
};

#endif
