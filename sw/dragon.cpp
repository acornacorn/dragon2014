#include <dragon.h>
#include <ac_counter.h>
#include <ac_printf.h>
#include <dragon_pins.h>
#include <dragon_buttons.h>
#include <dragon_leds.h>
#include <stdint.h>

static const bool DEBUG_BLINK = true;

void Dragon::init()
{
  initBlink();
  initServos();
}

void Dragon::update()
{
  updateBlink();
  updateServos();
}

const char *Dragon::modeString(Mode mode)
{
  const char *str[] = 
  {
    DRAGON_MODE_STRINGS
  };

  if ((int)mode >= 0 && (int)mode < (int)ARRAY_SIZE(str))
    return str[mode];
  return "???";
}

Dragon::Mode Dragon::setModeInternal(Mode mode)
{
  switch(mode)
  {
  case MODE_CNT:
    mode = MODE_TEST;
    // fall thru
  case MODE_TEST:
  case MODE_KEY_LEYE:
  case MODE_KEY_REYE:
  case MODE_KEY_LOOK:
  case MODE_KEY_LIPS:
  case MODE_KEY_R:
  case MODE_KEY_G:
  case MODE_KEY_B:
    return mode;

  case MODE_HAPPY:
    return mode;
  case MODE_SNARL:
    return mode;
  case MODE_ANGRY1:
    return mode;

  }

  return MODE_TEST;
}

void Dragon::setMode(Mode mode)
{
  if ((int)mode < 0)
    mode = (Mode)(MODE_CNT-1);
  if ((int)mode >= (int)MODE_CNT)
    mode = (Mode)0;

  mode = setModeInternal(mode);

  acPrintf("Switch to mode %s\n", modeString(mode));
  mode_ = mode;
}

void Dragon::setEyePos(int left, int right, int duration_millis)
{
  l_eye_pos_ = left;
  r_eye_pos_ = right;

  if (!l_eye_close_)
    sv_leye_.go(l_eye_pos_, duration_millis);
  if (!r_eye_close_)
    sv_reye_.go(r_eye_pos_, duration_millis);
}



// blink delay in ms
static uint16_t g_blink_times[] = 
{
  1270,
  510,
  1383,
  2093,
  1873,
  420,
  1632,
  2513,
  2814,
  2513,
  2313,
  1122,
  370,
  1771,
  1333,
  2355,
  2002,
  2552,
  2864,
  1961,
};


void Dragon::initBlink()
{
  enableBlink(true);
}

void Dragon::enableBlink(bool enable)
{
  blink_enable_ = enable;
  blink_index_ = 0;
  endBlink();
}

void Dragon::startBlink()
{
  // how long eyes stay closed
  static const int BLINK_TIME = 20;
  blink_timer_.init(BLINK_TIME);
  in_blink_ = true;
}

void Dragon::endBlink()
{
  in_blink_ = false;
  blink_index_++;
  if (blink_index_ >= (int)ARRAY_SIZE(g_blink_times))
    blink_index_ = 0;
  blink_timer_.init(g_blink_times[blink_index_]);
}

void Dragon::updateBlink()
{
  bool enable = blink_enable_ && mode_ > LAST_KEY_MODE;

  if (in_blink_)
  {
    if (!enable || blink_timer_.check())
    {
      endBlink();
    }
  }
  else if (enable && blink_timer_.check())
  {
    startBlink();
  }

  static const int EYE_CLOSE_DURATION = 0;
  static const int EYE_OPEN_DURATION = 0;

  if (in_blink_ || g_buttons_value[BUT_LEYE])
  {
    if (!l_eye_close_)
    {
      sv_leye_.go(sv_leye_closed, EYE_CLOSE_DURATION);
      l_eye_close_ = true;
    }
  }
  else if (l_eye_close_)
  {
    sv_leye_.go(l_eye_pos_, EYE_OPEN_DURATION);
    l_eye_close_ = false;
  }

  if (in_blink_ || g_buttons_value[BUT_REYE])
  {
    if (!r_eye_close_)
    {
      sv_reye_.go(sv_reye_closed, EYE_CLOSE_DURATION);
      r_eye_close_ = true;
    }
  }
  else if (r_eye_close_)
  {
    sv_reye_.go(r_eye_pos_, EYE_OPEN_DURATION);
    r_eye_close_ = false;
  }
}



void Dragon::debugIncrement(int inc)
{
  switch(g_dragon.getMode())
  {
  case MODE_KEY_LEYE:
    sv_leye_.debugIncrement(inc);
    break;
  case MODE_KEY_REYE:
    sv_reye_.debugIncrement(inc);
    break;
  case MODE_KEY_LOOK:
    sv_look_.debugIncrement(inc);
    break;
  case MODE_KEY_LIPS:
    sv_lips_.debugIncrement(inc);
    break;
  case MODE_KEY_R:
    g_eyes.debugIncrement(0, inc);
    break;
  case MODE_KEY_G:
    g_eyes.debugIncrement(1, inc);
    break;
  case MODE_KEY_B:
    g_eyes.debugIncrement(2, inc);
    break;
  default:
    break;
  }
}

void Dragon::initServos()
{
  sv_look_.init(dpin_look_servo,
                sv_look_mid,
                sv_look_min,
                sv_look_max);
  sv_leye_.init(dpin_leye_servo,
                sv_leye_mid,
                sv_leye_min,
                sv_leye_max);
  sv_reye_.init(dpin_reye_servo,
                sv_reye_mid,
                sv_reye_min,
                sv_reye_max);
  sv_lips_.init(dpin_lips_servo,
                sv_lips_mid,
                sv_lips_min,
                sv_lips_max);

  l_eye_pos_ = sv_leye_mid;
  r_eye_pos_ = sv_reye_mid;

  servo_test_cnt_.init(2000, 2);
}

void Dragon::updateServos()
{
  uint32_t now_millis = millis();
  sv_look_.update(now_millis);
  sv_leye_.update(now_millis);
  sv_reye_.update(now_millis);
  sv_lips_.update(now_millis);

  if (getMode() == MODE_TEST)
  {
    uint32_t val;
    if (servo_test_cnt_.check(&val))
    {
      if (val)
      {
        acPrintf("Down\n");
        sv_look_.go(0, 1500);
      }
      else
      {
        acPrintf("Up\n");
        sv_look_.go(255, 1500);
      }
    }
  }
}


