#include <dragon_buttons.h>
#include <dragon_pins.h>
#include <dragon.h>
#include <ac_printf.h>
#include <Arduino.h>
#include <Bounce.h>

static const bool DEBUG_KEYS = true;
static const bool DEBUG_BUTTONS = true;

static const int DEBOUNCE_TIME_MS = 20;


static Bounce* g_buttons[BUTTON_CNT];
bool g_buttons_changed[BUTTON_CNT];
bool g_buttons_value[BUTTON_CNT];

AcCounter g_help_cnt;
bool g_show_help = false;

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

  g_show_help = false;
  g_help_cnt.init(10, 2);
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

static const char *help[] = 
{
    "",
    "  __HELP__",
    "  l - leye servo",
    "  r - reye servo",
    "  o - look servo",
    "  i - lips servo",
    "  R - red",
    "  G - green",
    "  B - blue",
    "  , - decrement",
    "  . - increment",
    "  - - prev mode",
    "  = - next mode",
    "  p - happy",
    "  a - angry",
    0,
};

static void startHelp()
{
  g_show_help = true;
  g_help_cnt.init(11, 1000);
}

static void updateHelp()
{
  if (g_show_help)
  {
    uint32_t val;
    if (g_help_cnt.check(&val))
    {
      if (help[val] == 0)
      {
        g_show_help = false;
      }
      else
      {
        acPrintf("%s\n", help[val]);
      }
    }
  }
}

static void parseKey(int c)
{
  int inc = 0;
  switch(c)
  {
  case 'H':
  case 'h':
  case '/':
  case '?':
    startHelp();
    break;
  case 'l':
    g_dragon.setMode(Dragon::MODE_KEY_LEYE);
    break;
  case 'r':
    g_dragon.setMode(Dragon::MODE_KEY_REYE);
    break;
  case 'o':
    g_dragon.setMode(Dragon::MODE_KEY_LOOK);
    break;
  case 'i':
    g_dragon.setMode(Dragon::MODE_KEY_LIPS);
    break;
  case 'R':
    g_dragon.setMode(Dragon::MODE_KEY_R);
    break;
  case 'G':
    g_dragon.setMode(Dragon::MODE_KEY_G);
    break;
  case 'B':
    g_dragon.setMode(Dragon::MODE_KEY_B);
    break;
  case 'a':
    g_dragon.setMode(Dragon::MODE_ANGRY1);
    break;
  case 'p':
    g_dragon.setMode(Dragon::MODE_HAPPY);
    break;
  case '-':
  case '_':
    g_dragon.setMode((Dragon::Mode)(g_dragon.getMode() - 1));
    break;
  case '+':
  case '=':
    g_dragon.setMode((Dragon::Mode)(g_dragon.getMode() + 1));
    break;
  case ',':
  case '<':
    inc = -1;
    break;
  case '.':
  case '>':
    inc = +1;
    break;
  default:
    acPrintf("Unknown key: 0x%02x = '%c'\n",
      c,
      c >= ' ' && c < 0x7f ? c : '?');
    break;
  }

  if (inc)
    g_dragon.debugIncrement(inc);
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
  updateHelp();
}
