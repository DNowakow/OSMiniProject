#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "envstate.h"

struct spinlock envlock;
struct env_state global_env;

void
envstate_init(void)
{
  initlock(&envlock, "envlock");
  global_env.cpu_load    = 0;
  global_env.temperature = 25;
  global_env.tick_count  = 0;
}

void
envstate_update(int cpu_load, int temp)
{
  acquire(&envlock);
  global_env.cpu_load    = cpu_load;
  global_env.temperature = temp;
  global_env.tick_count++;
  release(&envlock);
}

void
envstate_read(struct env_state *out)
{
  acquire(&envlock);
  out->cpu_load    = global_env.cpu_load;
  out->temperature = global_env.temperature;
  out->tick_count  = global_env.tick_count;
  release(&envlock);
}