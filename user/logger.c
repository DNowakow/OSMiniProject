#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void
write_int(int fd, int n)
{
  char buf[16];
  int i = 0;

  if(n == 0){ write(fd, "0", 1); return; }
  if(n < 0) { write(fd, "-", 1); n = -n; }

  while(n > 0){ buf[i++] = '0' + (n % 10); n /= 10; }

  for(int a = 0, b = i-1; a < b; a++, b--){
    char tmp = buf[a]; buf[a] = buf[b]; buf[b] = tmp;
  }
  write(fd, buf, i);
}

int
main(int argc, char *argv[])
{
  int fd;
  struct env_state state;
  int i;

  printf("logger: opening envlog.txt\n");
  fd = open("envlog.txt", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf("logger: failed to open log file\n");
    exit(1);
  }

  write(fd, "tick,cpu_load,temperature\n", 26);

  for(i = 0; i < 30; i++){
    if(getsensordata(&state) < 0){
      printf("logger: getsensordata failed\n");
      break;
    }

    write_int(fd, state.tick_count);
    write(fd, ",", 1);
    write_int(fd, state.cpu_load);
    write(fd, ",", 1);
    write_int(fd, state.temperature);
    write(fd, "\n", 1);

    printf("logger: entry %d -- tick=%d cpu=%d temp=%d\n",
           i, state.tick_count, state.cpu_load, state.temperature);

    sleep(15);
  }

  close(fd);
  printf("logger: saved to envlog.txt\n");
  exit(0);
}