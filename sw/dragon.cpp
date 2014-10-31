#include <dragon.h>
#include <ac_counter.h>
#include <ac_printf.h>
#include <dragon_pins.h>
#include <dragon_buttons.h>
#include <dragon_leds.h>
#include <dragon_emotions.h>
#include <stdint.h>

static const bool DEBUG_BLINK = true;

void Dragon::init()
{
  initBlink();
  initLook();
  initServos();
  initEmoColors();
  setMode(MODE_KEY_R);
}

void Dragon::update()
{
  updateBlink();
  updateLook();
  updateServos();
  updateEmoColors();
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

void Dragon::initEmoColors()
{
  eye_color_cnt_.init(1000, 2);
}

void Dragon::updateEmoColors()
{
  uint32_t val;
  if (emotion_ && eye_color_cnt_.check(&val))
  {
    if (eye_color_transition_)
    {
      eye_color_cnt_.init(emotion_->color_period, 2);
      val = 0;
      eye_color_transition_ = false;
    }
    if (val)
    {
      dragonLedColorShiftTo(emotion_->color1[0],
                        emotion_->color1[1],
                        emotion_->color1[2],
                        emotion_->color_period);
    }
    else
    {
      dragonLedColorShiftTo(emotion_->color2[0],
                        emotion_->color2[1],
                        emotion_->color2[2],
                        emotion_->color_period);
    }
  }
}

void Dragon::setModeEmotion(Mode mode)
{
  emotion_ = getEmotion(mode);

  dragonLedColorShiftTo(emotion_->color1[0],
                        emotion_->color1[1],
                        emotion_->color1[2],
                        emotion_->transition_period);

  eye_color_cnt_.init(emotion_->transition_period, 2);
  eye_color_transition_ = true;

  sv_leye_.go(emotion_->leye, emotion_->transition_period);
  sv_reye_.go(emotion_->reye, emotion_->transition_period);
  sv_lips_.go(emotion_->lips, emotion_->transition_period);
}

Dragon::Mode Dragon::setModeInternal(Mode mode)
{
  switch(mode)
  {
  case MODE_CNT:
    mode = MODE_TEST;
    // fall thru
  case MODE_TEST:
  case MODE_BUT_SERVO_LIMITS:
  case MODE_KEY_LEYE:
  case MODE_KEY_REYE:
  case MODE_KEY_LOOK:
  case MODE_KEY_LIPS:
  case MODE_KEY_R:
  case MODE_KEY_G:
  case MODE_KEY_B:
    return mode;

  case MODE_HAPPY:
  case MODE_SNARL1:
  case MODE_SNARL2:
  case MODE_ANGRY1:
  case MODE_ANGRY2:
  case MODE_ANGRY3:
    setModeEmotion(mode);
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

void Dragon::incEmotion(int inc)
{
  int mode = mode_ + inc;
  mode = acClamp(mode, MODE_FIRST_EMOTION, MODE_LAST_EMOTION);
  setMode((Dragon::Mode)mode);
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
acPrintf("enter initBlnk\n");
  enableBlink(true);
acPrintf("exit initBlnk\n");
}

void Dragon::enableBlink(bool enable)
{
acPrintf("enter enableBlink %d -> %d\n",blink_enable_, enable);
  blink_enable_ = enable;
acPrintf("enter2 enableBlink %d -> %d\n",blink_enable_, enable);
  blink_index_ = 0;
acPrintf("enter3 enableBlink %d -> %d\n",blink_enable_, enable);
  endBlink();
acPrintf("exit enableBlink(%d)\n",blink_enable_);
}

void Dragon::startBlink()
{
//acPrintf("startBlink\n");
  // how long eyes stay closed
  static const int BLINK_TIME = 200;
  blink_timer_.init(BLINK_TIME);
  in_blink_ = true;
}

void Dragon::endBlink()
{
//acPrintf("enter1 endBlink  idx=%d enab=%d\n",blink_index_,blink_enable_);
  in_blink_ = false;
//acPrintf("enter2 endBlink  idx=%d enab=%d\n",blink_index_,blink_enable_);
  blink_index_++;
//acPrintf("enter3 endBlink  idx=%d enab=%d\n",blink_index_,blink_enable_);
  if (blink_index_ >= (int)ARRAY_SIZE(g_blink_times))
    blink_index_ = 0;
//acPrintf("enter4 endBlink  idx=%d enab=%d\n",blink_index_,blink_enable_);
  blink_timer_.init(g_blink_times[blink_index_]);
//acPrintf("exit endBlink  idx=%d enab=%d\n",blink_index_,blink_enable_);
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

  if (mode_ == MODE_BUT_SERVO_LIMITS)
    return;

  static const int EYE_CLOSE_DURATION = 0;
  static const int EYE_OPEN_DURATION = 0;

  if (in_blink_ || g_buttons_value[BUT_LEFT_BLINK])
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

  if (in_blink_ || g_buttons_value[BUT_RIGHT_BLINK])
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

void Dragon::look(int val, bool compensate)
{
  if (mode_ == MODE_KEY_LOOK)
    return;

  int old = sv_look_.getValue();

acPrintf("Look %d -> %d         (%d)\n",old,val, compensate);

  if (compensate && old < val)
  {
    val += 4;
  }
  if (compensate && old > val)
  {
    val -= 4;
  }

  sv_look_.go(val, 0);

  // backoff backs the servo off after a while to minimize noise and power

  look_backoff_ = val;

  if (old < val)
  {
    look_backoff_ = val - sv_look_backoff;
    look_backoff_ = acMax(look_backoff_, old);
  }
  else if (old > val)
  {
    look_backoff_ = val + sv_look_backoff;
    look_backoff_ = acMin(look_backoff_, old);
  }

  look_backoff_ = acClamp(look_backoff_, sv_look_min_ss, sv_look_max_ss);

  look_backoff_timer_.init(200);
}

void Dragon::initLook()
{
  look(sv_look_mid, false);
}

void Dragon::updateLook()
{
  if (mode_ == MODE_KEY_LOOK)
  {
    look_backoff_ = 0;
    return;
  }
  if (look_backoff_ && look_backoff_timer_.check())
  {
    sv_look_.go(look_backoff_, 0);
    look_backoff_ = 0;
  }
}



void Dragon::debugToggleServo(Mode servo)
{
  AcServo *s = NULL;
  switch(servo)
  {
  case MODE_KEY_LEYE:
    s = &sv_leye_;
    break;
  case MODE_KEY_REYE:
    s = &sv_reye_;
    break;
  case MODE_KEY_LOOK:
    s = &sv_look_;
    break;
  case MODE_KEY_LIPS:
    s = &sv_lips_;
    break;
  default:
    return;
  }


  int val = s->getValue();
  if (val > 107)
    val = 0;
  else
    val = 255;

  s->go(val, 400);
  acPrintf("Toggle servo %s to %d\n",
    modeString(servo), val);
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


