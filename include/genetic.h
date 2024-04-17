// genetic.h - A simple genetic algorithm implementation

#ifndef GENETIC_H
#define GENETIC_H

#include <assert.h>
#include <stdint.h>

struct ga_individual_t;

// ga_crossover_t
//
// Type definition for a crossover function
//
// uintptr_t * - The address for the child pointer
// uintptr_t - Parent pointer
// uintptr_t - Parent pointer

typedef void (*ga_crossover_t)(ga_individual_t *, ga_individual_t,
                               ga_individual_t);

// ga_mutation_t
//
// Type definition for a crossover function
//
// uintptr_t - The individual being mutated

typedef void (*ga_mutation_t)(ga_individual_t);

// ga_fitness_t()
//
// Type definition for a fitness function
//
// uintptr_t - The individual on which to run the fitness function
// void const * - The user defined data pointer

typedef double (*ga_fitness_t)(ga_individual_t, void const *);

double ga_generation(ga_individual_t *, uint32_t, double, double, ga_fitness_t,
                     void const *, ga_crossover_t, ga_mutation_t);

#endif // ga_H

#ifdef GENETIC_IMPLEMENTATION // IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ga_finess_sort()
//
// Run the fitness function on
//
// population - An array of pointers to each individual
// population_n - The size of the population array
// fitness - The function used to evaluate the fitness of an individual
// fitness_data - Additional data required for the fitness function
//
// return - The error for the best performing individual

static double ga_fitness_sort(ga_individual_t *population,
                              uint32_t population_n, ga_fitness_t fitness,
                              void const *fitness_data) {
  double *fitness_buffer = calloc(population_n, sizeof(double));
  double fitness_current = 0;
  ga_individual_t individual;

  // Insertion sort
  uint32_t i, j, k;
  for (i = 0; i < population_n; i++) {
    fitness_current = fitness(population[i], fitness_data);
    individual = population[i];

    // for (j = i; j > 0 && abs(fitness_current) > fitness_buffer[j - 1]; j--) {
    for (j = i; j > 0 && abs(fitness_current) < fitness_buffer[j - 1]; j--) {
    }

    // Shift array back by one for sorting
    for (k = i; k > j; k--) {
      population[k] = population[k - 1];
      fitness_buffer[k] = fitness_buffer[k - 1];
    }

    population[j] = individual;
    fitness_buffer[j] = fitness_current;
  }

  double error = fitness_buffer[0];
  free(fitness_buffer);

  return error;
}

// ga_generation()
//
// Run a single generation of the genetic algorithm
//
// population - An array of pointers to each individual
// population_n - The size of the population array
// rate_selection - A number [0,1] denoting the percentage of the population
//                  which survives to the next generation
// rate_mutation - A number [0,1] denoting the percentage of new individuals
//                 which will recieve mutations
// fitness - The function used to evaluate the fitness of an individual
// fitness_data - Additional data required for the fitness function
// crossover - The function used to perform repopulation via the
//              combination of two parents
// mutation - The function used to perform mutations on a given individual
//
// return - The error of the best performing individual

double ga_generation(ga_individual_t *population, uint32_t population_n,
                     double rate_selection, double rate_mutation,
                     ga_fitness_t fitness, void const *fitness_data,
                     ga_crossover_t crossover, ga_mutation_t mutation) {
  //   assert(0 < rate_selection && rate_selection <= 1.0);
  //   assert(0 < rate_mutation && rate_mutation <= 1.0);
  assert(2 < population_n);

  uint32_t selection_n = rate_selection * population_n;

  // Crossover
  for (uint32_t i = selection_n; i < population_n; i++) {
    uint32_t parent_i = rand() % selection_n;
    uint32_t parent_j = rand() % selection_n;

    // printf("%p - %p\n", &population[i], population[i]);
    crossover(&population[i], population[parent_i], population[parent_j]);
  }

  // Mutation
  uint32_t mutation_n = rate_mutation * (population_n - selection_n);
  for (uint32_t i = 0; i < mutation_n; i++) {
    uint32_t mutation_i = rand() % (population_n - selection_n) + selection_n;
    mutation(population[mutation_i]);
  }

  // Fitness
  return ga_fitness_sort(population, population_n, fitness, fitness_data);
}

#endif
