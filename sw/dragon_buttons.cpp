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
  bool none = true;

  for (int i = 0 ; i < BUTTON_CNT ; ++i)
  {
    g_buttons_changed[i] = g_buttons[i]->update();
    g_buttons_value[i] = !g_buttons[i]->read();

    if (g_buttons_changed[i] || g_buttons_value[i])
    {
      none = false;
    }

    if (DEBUG_BUTTONS && g_buttons_changed[i])
    {
      acPrintf("Button %d: %s\n",
        i,
        g_buttons_value[i] ? "Pressed" : "Released");
    }
  }

  // wait for buttons to settle after startup.  Ignore presses before that.
  static AcTimer startup;
  static int first = 1;
  if (first)
  {
    if (none)
    {
      first = 0;
      acPrintf("Buttons OK\n");
    }
    else if (first == 1)
    {
      startup.init(1000);
      first = 2;
    }
    else if (startup.check())
    {
      first = 0;
      acPrintf("Buttons TIMED OUT\n");
    }

    for (int i = 0 ; i < BUTTON_CNT ; ++i)
    {
      g_buttons_changed[i] = false;
      g_buttons_value[i] = false;
    }
    return;
  }


  // look left        BUT_LOOK_LEFT
  // look right       BUT_LOOK_RIGHT
  // look all over    BUT_LOOK_LEFT + BUT_LOOK_RIGHT
  //
  // emption+         BUT_EMO_INC
  // emotion-         BUT_EMO_DEC
  //
  // right blink      BUT_RIGHT_BLINK
  // left blink       BUT_LEFT_BLINK
  // toggle autoblink BUT_LEFT_BLINK + BUT_RIGHT_BLINK

  static bool both_look = false;
  static bool both_blink = false;

  // DETECT BUTTON PAIRS
  if (g_buttons_value[BUT_LOOK_LEFT] &&
      g_buttons_value[BUT_LOOK_RIGHT])
  {
    both_look = true;
  }
  if (g_buttons_value[BUT_RIGHT_BLINK] &&
      g_buttons_value[BUT_LEFT_BLINK])
  {
    // TOGGLE AUTO BLINK
    if (!both_blink)
    {
      g_dragon.enableBlink(!g_dragon.getEnableBlink());
    }

    both_blink = true;
  }

  if (g_buttons_changed[BUT_EMO_INC] &&
      g_buttons_value[BUT_EMO_INC])
  {
    g_dragon.incEmotion(1);
  }

  if (g_buttons_changed[BUT_EMO_DEC] &&
      g_buttons_value[BUT_EMO_DEC])
  {
    g_dragon.incEmotion(-1);
  }

  // NOTE: Blinking is handled in Dragon::updateBlink()

  // LOOK LEFT
  if (g_buttons_changed[BUT_LOOK_LEFT])
  {
    if (both_look)
    {
      g_dragon.look(sv_look_mid, true);
      // TODO: enable auto looking around mode
    }
    else if (g_buttons_value[BUT_LOOK_LEFT])
    {
      g_dragon.look(sv_look_left, false);
    }
    else
    {
      g_dragon.look(sv_look_mid, true);
    }
  }

  // LOOK RIGHT
  if (g_buttons_changed[BUT_LOOK_RIGHT])
  {
    if (both_look)
    {
      g_dragon.look(sv_look_mid, true);
      // TODO: enable auto looking around mode
    }
    else if (g_buttons_value[BUT_LOOK_RIGHT])
    {
      g_dragon.look(sv_look_right, false);
    }
    else
    {
      g_dragon.look(sv_look_mid, true);
    }
  }

  // DETECT BUTTON PAIRS
  if (!g_buttons_value[BUT_LOOK_LEFT] &&
      !g_buttons_value[BUT_LOOK_RIGHT])
  {
    both_look = false;
  }
  if (!g_buttons_value[BUT_RIGHT_BLINK] &&
      !g_buttons_value[BUT_LEFT_BLINK])
  {
    both_blink = false;
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
