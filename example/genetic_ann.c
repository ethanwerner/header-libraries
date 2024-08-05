#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ANN_IMPLEMENTATION
#include "../include/ann.h"

#define POPULATION_N 100
#define RATE_SELECTION 0.2
#define RATE_MUTATON 10
#define MAGNITUDE_MUTATION 0.01
#define ERROR 0.00001

typedef ann_t *ga_individual_t;

#define GENETIC_IMPLEMENTATION
#include "../include/genetic.h"

double fitness(ann_t *const x, void const *c) {
  fp_t *i = *((fp_t **)c);
  fp_t *t = *((fp_t **)c + 1);
  fp_t o[4];

  fp_t e = 0, sum = 0;
  for (uint_t b = 0; b < 4; b++) {
    ann_propagation_forward(x, &i[2 * b], &o[b]);
    e = ann_error_total(&o[b], &t[b], x->layer_neuron_n[x->layer_n - 1]);
    sum += e;
  }

  return sum;
}

void crossover(ann_t **y, ann_t *x0, ann_t *x1) {
  uint32_t i_crossover = rand() % x0->weight_n;
  memcpy((*y)->weight, x0->weight, i_crossover * sizeof(fp_t));
  memcpy((*y)->weight + i_crossover, x1->weight + i_crossover,
         (x0->weight_n - i_crossover) * sizeof(fp_t));
}

void mutate(ann_t *x) {
  uint32_t i_mutation = rand() % x->weight_n;
  fp_t r =
      (2 * (MAGNITUDE_MUTATION * (fp_t)rand() / RAND_MAX)) - MAGNITUDE_MUTATION;
  x->weight[i_mutation] += r;
}

int main(void) {
  srand(time(0));

  fp_t input[12] = {0, 0, 0, 1, 1, 0, 1, 1};
  fp_t output;
  fp_t target[4] = {0, 1, 1, 0};

  ann_t **networks = malloc(sizeof(ann_t *) * POPULATION_N);

  for (uint_t i = 0; i < POPULATION_N; i++) {
    networks[i] = ann_init(3, (uint_t[]){2, 2, 1});
    ann_set_activation(networks[i], SIGMOID, IDENTITY);
    ann_random(networks[i]);
  }

  fp_t e = ERROR + 1;
  uint32_t n = 0, i = 0;
  while (e > ERROR) {
    e = ga_generation(networks, POPULATION_N, RATE_SELECTION, RATE_MUTATON,
                      fitness, (fp_t *[2]){input, target}, crossover, mutate);

    n++;
    i++;

    if (i > 1000) {
      printf("\rERROR: %10.5lf", e);
      i = 0;
    }
  }

  printf("\33[2K\r");
  printf("ERROR: %lf", e);

  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 4; j++) {
      ann_propagation_forward(networks[i], (fp_t const *const)&input[2 * j],
                              &output);
      ann_print_neuron(networks[i], &input[2 * j], &output);
    }
  }

  printf("\nITERATIONS: %u\n", n);
}
