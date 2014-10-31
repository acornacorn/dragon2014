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

  BUT_LEYE = BUT0,
  BUT_REYE = BUT1,
};

extern bool g_buttons_changed[BUTTON_CNT];
extern bool g_buttons_value[BUTTON_CNT];

#endif
