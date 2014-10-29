#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>


#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

typedef struct SerialPort
{
  int fd;
  struct SerialPort *next;
  char *dev;
  int bufcnt;
  char buf[200];

  int empty_cnt;
} SerialPort;

SerialPort *g_serial_ports = NULL;

int g_debug = 0;

void showTermios(const struct termios *tio, const char *filename)
{
  speed_t ispeed = cfgetispeed(tio);
  speed_t ospeed = cfgetospeed(tio);

  printf("got termios for %s\n", filename);
  printf("  c_iflag = 0x%08x\n", tio->c_iflag);
  printf("  c_oflag = 0x%08x\n", tio->c_oflag);
  printf("  c_cflag = 0x%08x\n", tio->c_cflag);
  printf("  c_lflag = 0x%08x\n", tio->c_lflag);

  printf("  ispeed = %ld\n",(long)ispeed);
  printf("  ospeed = %ld\n",(long)ospeed);

  {
    int i;
    for (i = 0 ; i < NCCS ; ++i)
    {
      if (tio->c_cc[i] != 0)
      {
        printf("    c_cc[0x%03x] = %3d = %02x\n",
          i,
          tio->c_cc[i],
          tio->c_cc[i]);
      }
    }
  }
}

void setupStdin()
{
  const int fd = 0;  // stdin
  struct termios tio;
  int rv;

  if (g_debug)
    printf("Setting up stdin\n");

  //non-blocking IO
  errno = 0;
  rv = fcntl(fd, F_GETFL);
  if (rv == -1)
  {
    fprintf(stderr, "fcntl(stdin, F_GETFL) failed. %s\n",
      strerror(errno));
  }
  else
  {
    rv = fcntl(fd, F_SETFL, (long)(rv | O_NONBLOCK));
    if (rv == -1)
    {
      fprintf(stderr, "fcntl(stdin, F_SETFL) failed. %s\n",
        strerror(errno));
    }
  }

  errno = 0;
  rv = tcgetattr(fd, &tio);
  if (rv)
  {
    fprintf(stderr, "tcgetattr failed. %s\n",
      strerror(errno));
    return;
  }

  if (g_debug)
    showTermios(&tio, "stdin");

#if 0
  tio.c_iflag &= ~IGNBRK;
  tio.c_iflag |= INPCK;

  tio.c_cflag &= ~CREAD;

  #define FORCE_TERMIO_FLAGS 1
  if (FORCE_TERMIO_FLAGS)
  {
    tio.c_iflag = INPCK;
    tio.c_oflag = 0;
    tio.c_cflag = 0x8bd;
    tio.c_lflag = 0;
  }

  tio.c_cc[VTIME] = 0;
  tio.c_cc[VMIN] = 0;

  cfsetispeed(&tio, B9600);
  cfsetospeed(&tio, B9600);

  errno = 0;
  rv = tcsetattr(fd, TCSAFLUSH, &tio);

  if (rv)
  {
    fprintf(stderr, "tcsetattr failed.  %s\n",
      strerror(errno));
  }
#endif
}

// Default termios for ttyACM* device file:
//  c_iflag = 00000001
//  c_oflag = 00000000
//  c_cflag = 00000cbd
//  c_lflag = 00000000
//    c_cc[0x000] =   3 = 03
//    c_cc[0x001] =  28 = 1c
//    c_cc[0x002] = 127 = 7f
//    c_cc[0x003] =  21 = 15
//    c_cc[0x004] =   4 = 04
//    c_cc[0x005] =   5 = 05
//    c_cc[0x006] =   1 = 01
//    c_cc[0x008] =  17 = 11
//    c_cc[0x009] =  19 = 13
//    c_cc[0x00a] =  26 = 1a
//    c_cc[0x00c] =  18 = 12
//    c_cc[0x00d] =  15 = 0f
//    c_cc[0x00e] =  23 = 17
//    c_cc[0x00f] =  22 = 16


// Arduino sets ttyACM* as follows:
//  c_iflag = 0x00000010           -IGNBRK +INPCK
//  c_oflag = 0x00000000
//  c_cflag = 0x000008bd           -CREAD
//  c_lflag = 0x00000000
//  ispeed = B9600
//  ospeed = B9600
//    c_cc[0x000] =   3 = 03  VINTR
//    c_cc[0x001] =  28 = 1c  VQUIT
//    c_cc[0x002] = 127 = 7f  VERASE
//    c_cc[0x003] =  21 = 15  VKILL
//    c_cc[0x004] =   4 = 04  VEOF
//    c_cc[0x005] =   0 = 00  VTIME     default: 5
//    c_cc[0x006] =   0 = 00  VMIN      default: 1
//    c_cc[0x007] =   0 = 00  VSWTC
//    c_cc[0x008] =  17 = 11  VSTART
//    c_cc[0x009] =  19 = 13  VSTOP
//    c_cc[0x00a] =  26 = 1a  VSUSP
//    c_cc[0x00c] =  18 = 12  VREPRINT
//    c_cc[0x00d] =  15 = 0f  VDISCARD
//    c_cc[0x00e] =  23 = 17  VWERASE
//    c_cc[0x00f] =  22 = 16  VLNEXT



int openDeviceFile(const char *dev)
{
  int fd;
  struct termios tio;
  int rv;

  if (!dev)
    return -1;

  errno = 0;
  fd = open(dev, O_RDWR | O_CLOEXEC | O_NOCTTY | O_NONBLOCK);
  if (fd < 0)
  {
    fprintf(stderr, "Failed to open device '%s'.  %s\n",
      dev,
      strerror(errno));
    return -1;
  }

  if (g_debug)
    printf("Opened device %s\n",dev);

  errno = 0;
  rv = tcgetattr(fd, &tio);
  if (rv)
  {
    fprintf(stderr, "tcgetattr failed. %s\n",
      strerror(errno));
    close(fd);
    return -1;
  }

  if (g_debug)
    showTermios(&tio, dev);

  tio.c_iflag &= ~IGNBRK;
  tio.c_iflag |= INPCK;

  tio.c_cflag &= ~CREAD;

  #define FORCE_TERMIO_FLAGS 1
  if (FORCE_TERMIO_FLAGS)
  {
    tio.c_iflag = INPCK;
    tio.c_oflag = 0;
    tio.c_cflag = 0x8bd;
    tio.c_lflag = 0;
  }

  tio.c_cc[VTIME] = 0;
  tio.c_cc[VMIN] = 0;

  cfsetispeed(&tio, B9600);
  cfsetospeed(&tio, B9600);

  errno = 0;
  rv = tcsetattr(fd, TCSAFLUSH, &tio);

  if (rv)
  {
    fprintf(stderr, "tcsetattr failed.  %s\n",
      strerror(errno));
  }

  return fd;
}

void addSerialPort(const char *device_file)
{
  SerialPort *sp = calloc(1, sizeof(SerialPort));
  if (!sp)
    return;

  sp->dev = strdup(device_file);
  sp->fd = openDeviceFile(sp->dev);
  if (sp->fd < 0)
  {
    free(sp->dev);
    free(sp);
    return;
  }

  printf("Opened serial port: %s\n", sp->dev);

  sp->next = g_serial_ports;
  g_serial_ports = sp;
}

void removeSerialPort(const char *device_file)
{
  SerialPort **pp = &g_serial_ports;

  while(*pp)
  {
    if (!strcmp((*pp)->dev, device_file))
    {
      SerialPort *next = (*pp)->next;
      printf("Closed serial port: %s\n", (*pp)->dev);

      close((*pp)->fd);
      free((*pp)->dev);
      free(*pp);
      *pp = next;
    }
    else
    {
      pp = &(*pp)->next;
    }
  }
}

void removeAllSerialPorts()
{
  while(g_serial_ports)
  {
    removeSerialPort(g_serial_ports->dev);
  }
}

void addAllSerialPorts()
{
  DIR *dp = opendir("/dev");

  if (!dp)
  {
    fprintf(stderr, "Could not open dir '/dev'.  %s\n",
      strerror(errno));
    return;
  }

  for(;;)
  {
    struct dirent entry;
    struct dirent *result = NULL;
    int rv = readdir_r(dp, &entry, &result);
    if (rv)
    {
      fprintf(stderr, "Error reading /dev directory.\n");
      break;
    }
    if (!result)
      break;
    if (!strncmp(result->d_name, "ttyACM", 6))
    {
      char name[NAME_MAX];
      SerialPort *sp;

      snprintf(name, sizeof(name), "/dev/%s", result->d_name);

      for (sp = g_serial_ports ;; sp = sp->next)
      {
        if (!sp)
        {
          addSerialPort(name);
          break;
        }
        if (!strcmp(name, sp->dev))
          break;
      }
    }
  }

  closedir(dp);
}

int hexDigit(int nibble)
{
  nibble &= 0xf;
  if (nibble <= 9)
    return '0' + nibble;
  return 'a' + nibble - 10;
}

void doRead(SerialPort *sp)
{
  int gotcr = 0;
  int n;
  int save_errno;

  errno = 0;
  n = read(sp->fd, sp->buf + sp->bufcnt, sizeof(sp->buf) - sp->bufcnt - 1);
  save_errno = errno;

  if (n > 0)
  {
    int j = 0;
    char buf[250];
    sp->bufcnt += n;
    sp->empty_cnt = 0;

    for (n = 0 ; n < sp->bufcnt ; ++n)
    {
      int c = sp->buf[n];
      if (c == '\n')
      {
        gotcr = 0;
      }
      else if (c == '\r')
      {
        gotcr = 1;
      }
      else
      {
        if (gotcr)
        {
          buf[j++] = '\\';
          buf[j++] = 'x';
          buf[j++] = '0';
          buf[j++] = 'd';
          gotcr = 0;
        }
        if (sp->buf[n] < 0x20 || sp->buf[n] > 0x7e)
        {
          buf[j++] = '\\';
          buf[j++] = 'x';
          buf[j++] = hexDigit(c >> 4);
          buf[j++] = hexDigit(c & 0xf);
        }
        else
        {
          buf[j++] = c;
        }
      }

      if (c == '\n' || j > sizeof(buf) - 5)
      {
        buf[j++] = 0;
        if (0 && sp == g_serial_ports && !sp->next)
        {
          // only 1 serial port open
          printf("%s\n",
            buf);
        }
        else
        {
          printf("%s: %s\n",
            sp->dev,
            buf);
        }

        memmove(sp->buf, sp->buf + n + 1, sp->bufcnt - n - 1);
        sp->bufcnt -= n + 1;
        n = -1;
      }
    }
  }
  else if (n == 0)
  {
    sp->empty_cnt++;
    if (sp->empty_cnt > 5)
    {
      fprintf(stderr, "Closing %s which seems to be gone.\n",
        sp->dev);
      removeSerialPort(sp->dev);
    }
    return;
  }
  else
  {
    fprintf(stderr, "Error in read(%s).  %s\n",
      sp->dev,
      strerror(save_errno));
    fprintf(stderr, "Closing %s\n",
      sp->dev);
    removeSerialPort(sp->dev);
  }
}

void doSelect(void)
{
  fd_set fdr, fdw, fdx;
  int n;
  int cnt = 0;
  SerialPort *sp;
  struct timespec timeout;

  FD_ZERO(&fdr);
  FD_ZERO(&fdw);
  FD_ZERO(&fdx);

  for (sp = g_serial_ports ; sp ; sp = sp->next)
  {
    cnt = MAX(cnt, sp->fd + 1);
    FD_SET(sp->fd, &fdr);
  }

  timeout.tv_sec = 0;
  timeout.tv_nsec = 500000000;

  errno = 0;
  n = pselect(cnt, &fdr, &fdw, &fdx, &timeout, NULL);

  if (n > 0)
  {
    SerialPort *next;
    for (sp = g_serial_ports ; sp ; sp = next)
    {
      next = sp->next;
      if (FD_ISSET(sp->fd, &fdr))
      {
        doRead(sp);
      }
    }
  }
  else if (n == 0)
  {
    if (g_debug)
      printf("pselect returned 0\n");
    addAllSerialPorts();
  }
  else
  {
    fprintf(stderr, "Error in pselect.  %s\n",
      strerror(errno));
  }
}

void run()
{
  struct timeval last_check, now;
  long since;
  addAllSerialPorts();
  gettimeofday(&last_check, NULL);
  
  for(;;)
  {
    doSelect();

    gettimeofday(&now, NULL);

    since = now.tv_usec - last_check.tv_usec;
    since /= 1000;
    since += 1000 * (now.tv_sec - last_check.tv_sec);
    if (since > 1000)
    {
      last_check = now;
      addAllSerialPorts();
    }
  }
}

int main(int argc, char *argv[])
{
  setupStdin();

  //addSerialPort("/dev/ttyACM1");
  addAllSerialPorts();

  if (!g_serial_ports)
  {
    printf("Waiting for serial ports...\n");
  }

  run();

  removeAllSerialPorts();

  return 0;
}
