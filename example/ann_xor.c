// example_xor.c - Example XOR training

#define ANN_IMPLEMENTATION
#include "../include/ann.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  srand(time(0));

  ann_t *ann = ann_init(3, (uint_t[]){2, 2, 1});
  ann_set_activation(ann, SIGMOID, SIGMOID);
  ann_random(ann);

  fp_t input[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
  fp_t output[4] = {0, 0, 0, 0};
  fp_t expected[4] = {0, 1, 1, 0};
  fp_t error = 0;

  for (int_t iteration = 0; iteration < 100000; iteration++) {
    for (int_t training = 0; training < 4; training++) {
      ann_propagation_forward(ann, (fp_t const *const)&input[training],
                              &output[training]);

      error += ann_error_total(&output[training], &expected[training],
                               ann->layer_neuron_n[ann->layer_n - 1]);

      // Train using backpropagation
      ann_propagation_backward(ann, (fp_t const *const)&input[training],
                               &output[training], &expected[training], 0.4);

      // Train using numeric integration
      // ann_train_numeric( ann, ( fp_t const * const ) &i[b], &t[b], 0.03 );
    }
  }

  ann_print_weight(ann);

  fputs("------------------------------------------------------------\n\n",
        stderr);

  for (int i = 0; i < 4; i++) {
    ann_propagation_forward(ann, (fp_t const *const)&input[i], &output[i]);
    ann_print_neuron(ann, (fp_t const *const)&input[i], &output[i]);
  }
}
