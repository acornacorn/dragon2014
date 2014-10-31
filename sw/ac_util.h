#ifndef AC_UTIL_H
#define AC_UTIL_H

static inline int acMin(int a, int b)
{
  return a < b ? a : b;
}

static inline int acMax(int a, int b)
{
  return a > b ? a : b;
}

static inline int acClamp(int val, int mn, int mx)
{
  return val < mn ? mn :
         val > mx ? mx :
         val;

}

#endif

