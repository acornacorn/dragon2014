#ifndef DRAGON_BUTTONS_H
#define DRAGON_BUTTONS_H

void dragonButtonInit();
void dragonButtonUpdate();

enum {
  BUT0,
  BUT1,
  BUT2,
  BUT3,
  BUT4,
  BUT5,
  BUT6,
  BUT7,
  BUTTON_CNT,

  BUT_LEFT_BLINK = BUT0,
  BUT_RIGHT_BLINK = BUT1,

  // unused: BUT2
  // unused: BUT3

  BUT_EMO_INC = BUT4,
  BUT_EMO_DEC = BUT5,

  BUT_LOOK_LEFT = BUT6,
  BUT_LOOK_RIGHT = BUT7,
};

extern bool g_buttons_changed[BUTTON_CNT];
extern bool g_buttons_value[BUTTON_CNT];

#endif
