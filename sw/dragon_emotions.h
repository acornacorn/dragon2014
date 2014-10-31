#ifndef DRAGON_EMOTIONS_H
#define DRAGON_EMOTIONS_H

#include <stdint.h>
#include <dragon.h>

struct Emotion
{
  uint8_t leye;
  uint8_t reye;
  uint8_t lips;
  
  uint8_t color1[3];
  uint8_t color2[3];
  int color_period; // pulse period in msec

  // how long to take to transition to this emotion
  int transition_period;  // msec
};

// get emotion for Dragon::Mode
const Emotion *getEmotion(Dragon::Mode mode);

#endif

