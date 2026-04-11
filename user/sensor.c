#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

static unsigned long rand_state = 12345;

int
simple_rand(void)
{
  rand_state = rand_state * 1103515245 + 12345;
  return (int)((rand_state >> 16) & 0x7fff);
}

int
main(int argc, char *argv[])
{
  int cpu_load, temp;
  int i;

  printf("sensor: starting environmental sensor simulation\n");

  for(i = 0; i < 20; i++){
    cpu_load = 20 + (simple_rand() % 75);
    temp     = 30 + (simple_rand() % 60);

    printf("sensor: injecting cpu_load=%d temp=%d\n", cpu_load, temp);
    setsensordata(cpu_load, temp);

    sleep(10);
  }

  printf("sensor: done\n");
  exit(0);
}