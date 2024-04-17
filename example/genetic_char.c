// example_string.c - Example training of ann.h ANN, with genetic algorithm

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef char *ga_individual_t;

#define GENETIC_IMPLEMENTATION
#include "../include/genetic.h"

#define POPULATION_N 100
#define RATE_SELECTION 0.5
#define RATE_MUTATON 2.0
#define ERROR 0

#define TARGET_STRING                                                          \
  " !\"#$%&'()*+,-./"                                                          \
  "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"                         \
  "abcdefghijklmnopqrstuvwxyz{|}~"

#define generate() ((char)(rand() % (127 - 32) + 32))

void crossover(char **y, char *x0, char *x1) {
  uint32_t lx0 = strlen(x0);
  uint32_t lx1 = strlen(x1);

  uint32_t ix0 = rand() % lx0;
  uint32_t ix1 = rand() % lx1;

  uint32_t ly = ix0 + (lx1 - ix1);

  char *sy = malloc(ly + 1);
  strncpy(sy, x0, ix0);
  strncpy(sy + ix0, x1 + ix1, lx1 - ix1);
  sy[ly] = 0;

  free(*y);
  *y = sy;
}

void mutation(char *x) {
  uint32_t l = strlen(x);
  uint32_t i = rand() % l;

  x[i] = generate();
}

double fitness(char *individual, void const *data) {
  double error = 0;

  char const *target = (char const *)data;

  while (*individual && *target) {
    error += abs(*target++ - *individual++);
  }
  while (*individual) {
    error += *individual++;
  }
  while (*target) {
    error += *target++;
  }

  return error;
}

int main(void) {
  char const *target = TARGET_STRING;
  char *population[POPULATION_N];

  srand(time(0));

  // Initialize poplation
  for (uint64_t i = 0; i < POPULATION_N; i++) {
    uint64_t n = rand() % 5 + 20;
    char *str = malloc(n + 1);
    str[n] = (uint8_t)0;

    for (uint64_t j = 0; j < n; j++) {
      str[j] = generate();
    }

    population[i] = str;
  }

  printf("TARGET:  %s\n", TARGET_STRING);
  fflush(stdout);

  time_t t0 = clock();

  uint32_t n = 0;

  // Conduct opitmization
  double error = 1;
  while (error > ERROR) {
    error =
        ga_generation(population, POPULATION_N, RATE_SELECTION, RATE_MUTATON,
                      fitness, (void const *)target, crossover, mutation);

    n++;
  }

  time_t t1 = clock();

  printf("\nSOLUTION: %s", population[0]);
  printf("\nITERATIONS: %d\nTIME:       %f ms\n", n,
         1000.0 * (t1 - t0) / CLOCKS_PER_SEC);

  for (int i = 0; i < POPULATION_N; i++) {
    free(population[i]);
  }
}
