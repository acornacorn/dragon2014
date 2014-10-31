#ifndef DRAGON_H
#define DRAGON_H

#include <ac_servo.h>
#include <ac_timer.h>
#include <ac_counter.h>

class Dragon
{
public:
  void init();

  void update();

  #define DRAGON_MODE_STRINGS \
            "Test", \
                    \
            "Leye", \
            "Reye", \
            "Look", \
            "Lips", \
            "Red", \
            "Green", \
            "Blue", \
                    \
            "Happy", \
            "Snarl", \
            "Angry1", \

  enum Mode {
    MODE_TEST,

    // TEST modes must come before KEY modes
    LAST_TEST_MODE = MODE_TEST,

    MODE_KEY_LEYE,
    MODE_KEY_REYE,
    MODE_KEY_LOOK,
    MODE_KEY_LIPS,
    MODE_KEY_R,
    MODE_KEY_G,
    MODE_KEY_B,

    // KEY modes must come before real modes
    LAST_KEY_MODE = MODE_KEY_B,

    MODE_HAPPY,
    MODE_SNARL,
    MODE_ANGRY1,

    MODE_CNT,
  };

  void setMode(Mode mode);
  Mode getMode() { return mode_; }
  static const char *modeString(Mode mode);

  void setEyePos(int left, int right, int duration_millis);

  void enableBlink(bool enable);

  // increment or decrement based on mode
  void debugIncrement(int inc);

private:
  void initBlink();
  void updateBlink();
  void startBlink();
  void endBlink();

  void initServos();
  void updateServos();

  Mode setModeInternal(Mode mode);

  Mode mode_;

  // position of each eye, not counting blink
  int l_eye_pos_;
  int r_eye_pos_;

  bool l_eye_close_;
  bool r_eye_close_;

  bool blink_enable_;
  bool in_blink_;
  int blink_index_;
  AcTimer blink_timer_;

  AcServo sv_look_;
  AcServo sv_leye_;
  AcServo sv_reye_;
  AcServo sv_lips_;

  AcCounter servo_test_cnt_;
};

extern Dragon g_dragon;

#endif
