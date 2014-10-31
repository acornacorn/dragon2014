#include <dragon_emotions.h>
#include <dragon_pins.h>

#define C(r,g,b) {r,g,b}

static Emotion default_emotion =
{
  .leye = sv_leye_mid,
  .reye = sv_reye_mid,
  .lips = sv_lips_mid,
  
  .color1 = COLOR_GREEN,
  .color2 = COLOR_GREEN,
  .color_period = color_pulse_period,

  .transition_period = 700,
};

static Emotion happy =
{
  .leye = sv_leye_wide_open,
  .reye = sv_reye_wide_open,
  .lips = sv_lips_soft_max,
  
  .color1 = COLOR_GREEN,
  .color2 = COLOR_GREEN2,
  .color_period = color_pulse_period,

  .transition_period = 700,
};

static Emotion snarl1 =
{
  .leye = sv_leye_droopy,
  .reye = sv_reye_droopy,
  .lips = sv_lips_back_teeth_open,
  
  .color1 = COLOR_ANGRY_RED,
  .color2 = {80, 0, 0},
  .color_period = color_pulse_period,

  .transition_period = 600,
};

static Emotion snarl2 =
{
  .leye = (sv_leye_droopy + sv_leye_guarded) / 2,
  .reye = (sv_reye_droopy + sv_reye_guarded) / 2,
  .lips = sv_lips_nose_open,
  
  .color1 = COLOR_ANGRY_RED,
  .color2 = COLOR_GOLD,
  .color_period = color_pulse_period,

  .transition_period = 500,
};

static Emotion angry1 =
{
  .leye = sv_leye_guarded,
  .reye = sv_reye_guarded,
  .lips = 100,
  
  .color1 = COLOR_GOLD,
  .color2 = COLOR_GOLD_ORANGE,
  .color_period = color_pulse_period,

  .transition_period = 400,
};

static Emotion angry2 =
{
  .leye = (sv_leye_guarded + sv_leye_angry) / 2,
  .reye = (sv_reye_guarded + sv_reye_angry) / 2,
  .lips = 130,
  
  .color1 = COLOR_ICE_BLUE,
  .color2 = COLOR_ICE_VERY_BLUE,
  .color_period = color_pulse_period,

  .transition_period = 400,
};

static Emotion angry3 =
{
  .leye = sv_leye_angry,
  .reye = sv_reye_angry,
  .lips = 80,
  
  .color1 = COLOR_ANGRY_RED,
  .color2 = COLOR_MAGENTA,
  .color_period = color_pulse_period / 3,

  .transition_period = 400,
};


const Emotion *getEmotion(Dragon::Mode mode)
{
  switch(mode)
  {
  case Dragon::MODE_HAPPY: return &happy;
  case Dragon::MODE_SNARL1: return &snarl1;
  case Dragon::MODE_SNARL2: return &snarl2;
  case Dragon::MODE_ANGRY1: return &angry1;
  case Dragon::MODE_ANGRY2: return &angry2;
  case Dragon::MODE_ANGRY3: return &angry3;

  case Dragon::MODE_TEST:
  case Dragon::MODE_KEY_LEYE:
  case Dragon::MODE_KEY_REYE:
  case Dragon::MODE_KEY_LOOK:
  case Dragon::MODE_KEY_LIPS:
  case Dragon::MODE_KEY_R:
  case Dragon::MODE_KEY_G:
  case Dragon::MODE_KEY_B:
  case Dragon::MODE_CNT:
    break;
  }

  return &default_emotion;
}





