#include <ac_printf.h>
#include <ac_timer.h>
#include <Arduino.h>
#include <stdarg.h>

static AcTimer g_delay;

static const int FLUSH_DELAY = 2;

void acPrintf(const char *fmt, ...)
{
  static bool first = true;
  if (first)
  {
    first = false;
  }
  else
  {
    g_delay.wait();
  }
  g_delay.init(FLUSH_DELAY);

  char buf[100];

  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  Serial.print(buf);
  Serial.flush();
}

