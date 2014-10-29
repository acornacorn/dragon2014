#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


void showTermios(const struct termios *tio)
{

  printf("c_iflag = 0x%08x\n", tio->c_iflag);
  printf("c_oflag = 0x%08x\n", tio->c_oflag);
  printf("c_cflag = 0x%08x\n", tio->c_cflag);
  printf("c_lflag = 0x%08x\n", tio->c_lflag);

  {
    int i;
    for (i = 0 ; i < NCCS ; ++i)
    {
      if (tio->c_cc[i] != 0)
      {
        printf("  c_cc[0x%03x] = %3d = %02x\n",
          i,
          tio->c_cc[i],
          tio->c_cc[i]);
      }
    }
  }
}

// Arduino sets ttyACM* as follows:
//  c_iflag = 0x00000010
//  c_oflag = 0x00000000
//  c_cflag = 0x000008bd
//  c_lflag = 0x00000000
//    c_cc[0x000] =   3 = 03
//    c_cc[0x001] =  28 = 1c
//    c_cc[0x002] = 127 = 7f
//    c_cc[0x003] =  21 = 15
//    c_cc[0x004] =   4 = 04
//    c_cc[0x008] =  17 = 11
//    c_cc[0x009] =  19 = 13
//    c_cc[0x00a] =  26 = 1a
//    c_cc[0x00c] =  18 = 12
//    c_cc[0x00d] =  15 = 0f
//    c_cc[0x00e] =  23 = 17
//    c_cc[0x00f] =  22 = 16



int setup(const char *dev)
{
  int fd = open(dev, O_RDWR | O_CLOEXEC | O_NOCTTY | O_NONBLOCK);
  struct termios tio_orig;
  int rv;

  if (fd < 0)
  {
    fprintf(stderr, "Failed to open device '%s'\n",dev);
    return -1;
  }

  printf("Opened device\n");

  rv = tcgetattr(fd, &tio_orig);

  printf("got termios  rv=%d\n",rv);
  showTermios(&tio_orig);

  return fd;
}

int main(int argc, char *argv[])
{
  int fd = setup("/dev/ttyACM1");

  if (fd >= 0)
    close(fd);

  return 0;
}
