#include <ac_printf.h>
#include <Arduino.h>
#include <stdarg.h>

void acPrintf(const char *fmt, ...)
{
  char buf[100];

  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  Serial.print(buf);
  Serial.flush();
  delay(10);
}

