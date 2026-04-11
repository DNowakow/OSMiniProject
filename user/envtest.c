#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int passed = 0;
int failed = 0;

void
assert_eq(const char *name, int got, int expected)
{
  if(got == expected){
    printf("  PASS: %s\n", name);
    passed++;
  } else {
    printf("  FAIL: %s -- got %d, expected %d\n", name, got, expected);
    failed++;
  }
}

void
assert_range(const char *name, int val, int lo, int hi)
{
  if(val >= lo && val <= hi){
    printf("  PASS: %s (%d in [%d,%d])\n", name, val, lo, hi);
    passed++;
  } else {
    printf("  FAIL: %s -- %d not in [%d,%d]\n", name, val, lo, hi);
    failed++;
  }
}

int
main(void)
{
  struct env_state state;

  printf("\n=== envtest: Environmental Monitoring Unit Tests ===\n\n");

  printf("T-01: setsensordata / getsensordata round-trip\n");
  setsensordata(55, 65);
  getsensordata(&state);
  assert_eq("cpu_load stores correctly", state.cpu_load, 55);
  assert_eq("temperature stores correctly", state.temperature, 65);

  printf("\nT-02: tick_count increments on each update\n");
  int before = state.tick_count;
  setsensordata(40, 50);
  getsensordata(&state);
  assert_eq("tick_count incremented by 1", state.tick_count, before + 1);

  printf("\nT-03: cpu_load clamped to valid range\n");
  setsensordata(200, 30);
  getsensordata(&state);
  assert_range("cpu_load clamped to <=100", state.cpu_load, 0, 100);

  printf("\nT-04: temperature clamped to valid range\n");
  setsensordata(50, 999);
  getsensordata(&state);
  assert_range("temperature clamped to <=150", state.temperature, 0, 150);

  printf("\nT-05: rapid sequential updates (spinlock stress)\n");
  for(int i = 0; i < 100; i++){
    setsensordata(i % 100, i % 150);
  }
  getsensordata(&state);
  assert_range("cpu_load valid after 100 updates", state.cpu_load, 0, 100);
  assert_range("temperature valid after 100 updates", state.temperature, 0, 150);

  printf("\nT-06: negative inputs clamped to 0\n");
  setsensordata(-10, -50);
  getsensordata(&state);
  assert_eq("negative cpu_load clamped to 0", state.cpu_load, 0);
  assert_eq("negative temperature clamped to 0", state.temperature, 0);

  printf("\nT-07: null pointer returns -1 safely\n");
  int ret = getsensordata(0);
  assert_eq("getsensordata(null) returns -1", ret, -1);

  printf("\n=== Results: %d passed, %d failed ===\n\n", passed, failed);
  exit(failed > 0 ? 1 : 0);
}