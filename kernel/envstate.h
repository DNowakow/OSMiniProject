#ifndef ENVSTATE_H
#define ENVSTATE_H

struct env_state {
  int cpu_load;    // 0-100 percent
  int temperature; // 0-150 degrees (simulated)
  int tick_count;  // increments on every update
};

#endif