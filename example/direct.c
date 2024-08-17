typedef double value_t;
#define DT_IMPLEMENTATION
#include "../include/direct.h"
#include <stdio.h>

int main() {
  node_t x0 = {.x = 0.0, .function = dt_add};
  node_t x1 = {.x = 0.0, .function = dt_one};
  node_t x2 = {.x = 0.0, .function = dt_add};
  node_t x3 = {.x = 0.0, .function = dt_add};
  node_t x4 = {.x = 0.0, .function = dt_add};
  node_t x5 = {.x = 0.0, .function = dt_add};
  node_t x6 = {.x = 0.0, .function = dt_one};

  dt_connect(&x0, &x2);
  dt_connect(&x0, &x1);
  dt_connect(&x0, &x3);
  dt_connect(&x2, &x1);
  dt_connect(&x2, &x4);
  dt_connect(&x3, &x4);
  dt_connect(&x4, &x5);
  dt_connect(&x4, &x6);
  dt_connect(&x5, &x2);

  dt_update_trace(&x0);
  printf("data: %lf\n", x0.x);
  dt_update_trace(&x0);
  printf("data: %lf\n", x0.x);
  dt_update_trace(&x0);
  printf("data: %lf\n", x0.x);

  return 0;
}