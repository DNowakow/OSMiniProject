#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  struct env_state state;
  int i;

  printf("monitor: reading environmental sensor data\n");

  for(i = 0; i < 10; i++){
    if(getsensordata(&state) < 0){
      printf("monitor: getsensordata failed\n");
      exit(1);
    }
    printf("monitor: cpu_load=%d  temperature=%d  updates=%d\n",
           state.cpu_load, state.temperature, state.tick_count);
    sleep(20);
  }

  printf("monitor: done\n");
  exit(0);
}