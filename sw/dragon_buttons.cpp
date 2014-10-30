#include <dragon_buttons.h>
#include <dragon_pins.h>
#include <ac_printf.h>
#include <Arduino.h>
#include <Bounce.h>

static const bool DEBUG_KEYS = true;
static const bool DEBUG_BUTTONS = true;

static const int DEBOUNCE_TIME_MS = 20;



static const int BUTTON_CNT = 8;
static Bounce* g_buttons[BUTTON_CNT];
bool g_buttons_changed[BUTTON_CNT];
bool g_buttons_value[BUTTON_CNT];

static void initButton(
      int index,
      int pin,
      Bounce *bounce)
{
  g_buttons[index] = bounce;
  g_buttons_changed[index] = false;
}

#define INIT_BUTTON(index) \
  do \
  { \
    static Bounce but##index ( dpin_button##index, DEBOUNCE_TIME_MS ); \
    g_buttons[index] = &but##index; \
    g_buttons_changed[index] = false; \
    g_buttons_value[index] = false; \
    pinMode(dpin_button##index, INPUT_PULLUP); \
  } while(0)

static void initButtons()
{
  INIT_BUTTON(0);
  INIT_BUTTON(1);
  INIT_BUTTON(2);
  INIT_BUTTON(3);
  INIT_BUTTON(4);
  INIT_BUTTON(5);
  INIT_BUTTON(6);
  INIT_BUTTON(7);
}

static void updateButtons()
{
  for (int i = 0 ; i < BUTTON_CNT ; ++i)
  {
    g_buttons_changed[i] = g_buttons[i]->update();
    g_buttons_value[i] = !g_buttons[i]->read();

    if (DEBUG_BUTTONS && g_buttons_changed[i])
    {
      acPrintf("Button %d: %s\n",
        i,
        g_buttons_value[i] ? "Pressed" : "Released");
    }
  }
}

static void parseKey(int c)
{

}

static void checkKey()
{
  int c = Serial.peek();
  if (c != -1)
  {
    c = Serial.read();
    if (DEBUG_KEYS)
      acPrintf("Got key: %02x = %c\n", c, c >= ' ' && c < 0x7f ? c : '?');
    parseKey(c);
  }  
}


void dragonButtonInit()
{
  initButtons();
}

void dragonButtonUpdate()
{
  checkKey();
  updateButtons();
}
