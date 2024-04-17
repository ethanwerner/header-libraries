// ann.h - Artificial Neural Nework Libray

#ifndef ANN_H
#define ANN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef double fp_t;
typedef uint32_t uint_t;
typedef int32_t int_t;

typedef enum {
  IDENTITY,
  BINARY,
  SIGMOID,
  RELU,
  ELU,
  LRELU,
  TANH,
} ann_activation_t;

typedef struct {
  // The full size of the allocated structure
  uint_t n;

  // The number of layers in the neural network
  uint_t layer_n;

  // The neuron count ( hidden )
  uint_t neuron_n;

  // The total number of weights and biases
  uint_t weight_n;

  // The number of neurons in each layer
  uint_t *layer_neuron_n;

  // The pointer to the neurons
  fp_t *neuron;

  // The weights and biases for each neuron
  //   - w_lji denotes the connection between the jth neuron in layer l, and
  //     the ith neuron in the previous layer
  //   - b_lj denotes the bias for the jth neuron in layer l
  // { w_000, w_001, w_002, .... , b_00, ...., w_ijk, b_ij }`
  fp_t *weight;

  // The delta between between the actual and the cost function
  fp_t *delta;

  // The activation function used in the hidden layer neurons
  fp_t (*activation_hidden)(fp_t);

  // The partial derivative of the activation function used in the hidden
  // layer neurons
  fp_t (*activation_hidden_partial)(fp_t);

  // The activation function used in the ouput neurons
  fp_t (*activation_output)(fp_t);

  // The partial derivative of the activation function used in the output
  // layer neurons
  fp_t (*activation_output_partial)(fp_t);
} ann_t;

ann_t *ann_init(uint_t, uint_t *);
ann_t *ann_copy(ann_t const *);
void ann_free(ann_t *);
void ann_random(ann_t *);

void ann_propagation_forward(ann_t *, fp_t const *const, fp_t *);
void ann_propagation_backward(ann_t *, fp_t const *, fp_t *, fp_t const *,
                              fp_t);
void ann_train_numeric(ann_t *, fp_t const *, fp_t const *, fp_t);

fp_t ann_error_total(fp_t const *, fp_t const *, uint_t);
void ann_set_activation(ann_t *, ann_activation_t, ann_activation_t);

void ann_print_weight(ann_t *);
void ann_print_neuron(ann_t *, fp_t const *const, fp_t const *const);

#ifdef __cplusplus
}
#endif

#endif // ANN_H

////////////////////////////////////////////////////////////////////////////////
// IMPELEMENTATION SECTION
////////////////////////////////////////////////////////////////////////////////

#ifdef ANN_IMPLEMENTATION

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#define PRINT_PRECISION 10

#define SQUARE_ROOT_2 1.4142135623730950488016887242096
#define PI 3.1415926535897932384626433832795
#define SQUARE_ROOT_PI 1.7724538509055160272981674833411
#define SQUARE_ROOT_2_OVER_PI 0.7978845608028653558798921198687

#define ELU_ALPHA 0.2
#define LRELU_ALPHA 0.2

static fp_t ann_random_range(fp_t, fp_t);

static fp_t ann_error(fp_t, fp_t);
static fp_t ann_error_partial(fp_t, fp_t);

static fp_t ann_activation_identity(fp_t);
static fp_t ann_activation_identity_partial(fp_t);
static fp_t ann_activation_binary(fp_t);
static fp_t ann_activation_binary_partial(fp_t);
static fp_t ann_activation_sigmoid(fp_t);
static fp_t ann_activation_sigmoid_partial(fp_t);
static fp_t ann_activation_relu(fp_t);
static fp_t ann_activation_relu_partial(fp_t);
static fp_t ann_activation_elu(fp_t);
static fp_t ann_activation_elu_partial(fp_t);
static fp_t ann_activation_lrelu(fp_t);
static fp_t ann_activation_lrelu_partial(fp_t);
static fp_t ann_activation_tanh(fp_t);
static fp_t ann_activation_tanh_partial(fp_t);

static fp_t (*ACTIVATION[][2])(fp_t) = {
    {ann_activation_identity, ann_activation_identity_partial}, // IDENTITY
    {ann_activation_binary, ann_activation_binary_partial},     // BINARY
    {ann_activation_sigmoid, ann_activation_sigmoid_partial},   // SIGMOID
    {ann_activation_relu, ann_activation_relu_partial},         // RELU
    {ann_activation_elu, ann_activation_elu_partial},           // ELU
    {ann_activation_lrelu, ann_activation_lrelu_partial},       // LRELU
    {ann_activation_tanh, ann_activation_tanh_partial},         // TANH
};

// ann_init()
//
// Initialize the neural network
//
// layer_n - The layer count for the layer count
// layer_neuron_n - An array of neuron counts for each layer
//
// return - The created ann_t instance

ann_t *ann_init(uint_t layer_n, uint_t *layer_neuron_n) {
  assert(layer_n >= 2);

  uint_t neuron_n = 0;
  uint_t weight_n = 0;

  // Calculate the number of neurons and weights / biases
  uint_t l = 1;
  for (; l < layer_n - 1; l++) {
    neuron_n += layer_neuron_n[l];
    weight_n += layer_neuron_n[l] * (layer_neuron_n[l - 1] + 1);
  }

  weight_n += layer_neuron_n[l] * (layer_neuron_n[l - 1] + 1);

  uint_t n =
      sizeof(ann_t) +              // ANN
      (sizeof(uint_t) * layer_n) + // layer_neuron_n[]
      (sizeof(fp_t) * (neuron_n +  // neuron[]
                       weight_n +  // weight[]
                       neuron_n + layer_neuron_n[layer_n - 1])); // delta[]

  // Allocate everything as one structure
  ann_t *ann = (ann_t *)malloc(n);

  // ann_t | layer_neuron_n[] | neuron[] | weight[] | delta[]
  ann->n = n;
  ann->layer_n = layer_n;
  ann->weight_n = weight_n;
  ann->neuron_n = neuron_n;
  ann->layer_neuron_n = (uint_t *)((uint8_t *)ann + sizeof(ann_t));
  memcpy(ann->layer_neuron_n, layer_neuron_n, sizeof(uint_t) * layer_n);
  ann->neuron = (fp_t *)(ann->layer_neuron_n + ann->layer_n);
  ann->weight = ann->neuron + ann->neuron_n;
  ann->delta = ann->weight + ann->weight_n;

  ann_set_activation(ann, SIGMOID, SIGMOID);

  return ann;
}

// ann_free()
//
// Free the neural network's memory
//
// ann - The instance of ann_t to free

void ann_free(ann_t *ann) { free(ann); }

// ann_copy()
//
// Make a copy of the given ann_t instance
//
// ann - The instance of ann_t to copy
//
// return - The copied ann_t instance

ann_t *ann_copy(ann_t const *ann) {
  ann_t *copy = (ann_t *)malloc(ann->n);
  memcpy(copy, ann, ann->n);

  return copy;
}

// ann_propagation_forward()
//
// Perform forward propagation on the ann_t instance
//
// ann - The ann_t instance to perform the propagation on
// input - An array containing the input vector
// output - The destination array for the output vector

void ann_propagation_forward(ann_t *ann, fp_t const *const input,
                             fp_t *output) {
  // printf("ann:\t\t%p\n", ann);
  // printf("output:\t\t%p\n", output);
  fp_t *w_ij = ann->weight;
  // printf("ann->weight:\t%p\n", w_ij);
  fp_t const *x = input; // Input neuron into y
  fp_t *y = ann->neuron; // The current neuron being calculated

  uint_t l = 1;

  for (; l < ann->layer_n - 1; l++) {
    for (uint_t j = 0; j < ann->layer_neuron_n[l]; j++) {
      // printf("neuron[%u]:\t%p\n", j, &(y[j]));
      y[j] = 0;
      for (uint_t i = 0; i < ann->layer_neuron_n[l - 1]; i++) {
        y[j] += x[i] * *w_ij++;
      }

      y[j] += *w_ij++;
      y[j] = ann->activation_hidden(y[j]);
    }

    x = y;
    y += ann->layer_neuron_n[l];
  }

  // Last layer
  for (uint_t j = 0; j < ann->layer_neuron_n[l]; j++) {
    // printf("output[%u]:\t%p\n", j, &(output[j]));
    output[j] = 0;

    for (uint_t i = 0; i < ann->layer_neuron_n[l - 1]; i++) {
      output[j] += x[i] * *w_ij++;
    }

    output[j] += *w_ij++;
    output[j] = ann->activation_output(output[j]);
  }
}

// ann_propagation_backward()
//
// Perform backpropagation on the ann_t instance
//
// ann - The ann_t instance to perform backpropagation upon
// input - Input vector array
// output - Output vector array
// target - Target output vector array
// rate - Learning rate

void ann_propagation_backward(ann_t *ann, fp_t const *input, fp_t *output,
                              fp_t const *target, fp_t rate) {
  int_t l = ann->layer_n - 1;
  uint_t i, j, q;

  // First output layer delta
  fp_t *d_j = ann->delta + ann->neuron_n;

  // Output Deltas
  for (j = 0; j < ann->layer_neuron_n[l]; j++) {
    d_j[j] = ann->activation_output_partial(output[j]) *
             ann_error_partial(output[j], target[j]);
  }

  // First weight in the set between the last layer and the current
  fp_t *w_jq = ann->weight + ann->weight_n -
               ann->layer_neuron_n[l] * ann->layer_neuron_n[l - 1] -
               ann->layer_neuron_n[l];

  fp_t *o_j = ann->neuron + ann->neuron_n;
  fp_t *d_q;

  l--;

  // Hidden Deltas
  for (; l > 0; --l) {
    d_q = d_j;
    d_j -= ann->layer_neuron_n[l];
    o_j -= ann->layer_neuron_n[l];

    for (j = 0; j < ann->layer_neuron_n[l]; j++) {
      d_j[j] = 0;

      for (q = 0; q < ann->layer_neuron_n[l + 1]; q++) {
        d_j[j] += w_jq[q * (ann->layer_neuron_n[l] + 1) + j] * d_q[q];
      }

      d_j[j] *= ann->activation_hidden_partial(o_j[j]);
    }

    w_jq -= (ann->layer_neuron_n[l - 1] * ann->layer_neuron_n[l] +
             ann->layer_neuron_n[l - 1]);
  }

  fp_t *w_ij = ann->weight;
  d_j = ann->delta;

  l = 1;

  // Input training
  for (j = 0; j < ann->layer_neuron_n[l]; j++) {
    for (i = 0; i < ann->layer_neuron_n[l - 1]; i++) {
      *w_ij -= rate * input[i] * d_j[j];
      w_ij++;
    }

    *w_ij -= rate * d_j[j];
    w_ij++;
  }

  l++;
  fp_t *i_i = ann->neuron;

  // Hidden training
  for (; l < (int_t)ann->layer_n; l++) {
    d_j += ann->layer_neuron_n[l - 1];

    for (j = 0; j < ann->layer_neuron_n[l]; j++) {
      for (i = 0; i < ann->layer_neuron_n[l - 1]; i++) {
        *w_ij -= rate * i_i[i] * d_j[j];
        w_ij++;
      }

      *w_ij -= rate * d_j[j];
      w_ij++;
    }

    i_i += ann->layer_neuron_n[l - 1];
  }
}

// ann_activation_identity()
//
// y = x
// dy/dx = 1

static fp_t ann_activation_identity(fp_t x) { return x; }
static fp_t ann_activation_identity_partial(fp_t x) { return 1; }

// ann_activation_binary()
//
// y = { x < 0 -> 0, x > 0 -> 1 }
// dy/dx = 0

static fp_t ann_activation_binary(fp_t x) {
  return (x > 0.0) ? 1.0 : (x < 0.0) ? -1.0 : 0.0;
}
static fp_t ann_activation_binary_partial(fp_t x) { return 0.0; }

// ann_activation_sign()
//
// y = { x < 0 -> -1, x = 0 -> 0, x > 0 -> 1 }
// dy/dx = 0

static fp_t ann_activation_sign(fp_t x) { return (x > 0.0) ? 1.0 : 0.0; }
static fp_t ann_activation_sign_partial(fp_t x) { return 0.0; }

// ann_activation_sigmoid()
//
// y = 1.0 / ( 1.0 + e^-x )
// dy/dx = f(x) * ( 1.0 - f(x) )

static fp_t ann_activation_sigmoid(fp_t x) { return 1.0 / (1.0 + exp(-x)); }
static fp_t ann_activation_sigmoid_partial(fp_t x) { return x * (1.0 - x); }

// ann_activation_relu()
//
// y = { x < 0 -> 0, x >= 0 -> x }
// dy/dx = { x < 0 -> 0, x >= 0 -> 1 }

static fp_t ann_activation_relu(fp_t x) { return (x > 0.0) ? x : 0.0; }
static fp_t ann_activation_relu_partial(fp_t x) {
  return (x > 0.0) ? 1.0 : 0.0;
}

// ann_activation_elu()
//
// y = { x < 0 -> a * ( e^x - 1 ), x >= 0 -> x }
// dy/dx = { x < 0 -> x + a, x >= 0 -> 1 }

static fp_t ann_activation_elu(fp_t x) {
  return (x > 0.0) ? x : ELU_ALPHA * (expm1(x));
}
static fp_t ann_activation_elu_partial(fp_t x) {
  return (x > 0.0) ? 1.0 : x + ELU_ALPHA;
}

// ann_activation_lelu()
//
// y = { x < 0 -> a * x, x >= 0 -> x }
// dy/dx = { x < 0 -> a, x >= 0 -> 1 }

static fp_t ann_activation_lrelu(fp_t x) {
  return (x > 0.0) ? x : LRELU_ALPHA * x;
}
static fp_t ann_activation_lrelu_partial(fp_t x) {
  return (x > 0.0) ? 1.0 : LRELU_ALPHA;
}

// ann_activation_tanh()
//
// y = sinh( x ) / cosh( x ) = ( e^2x - 1 ) / ( e^2x + 1 )
// dy/dx = 1.0 - f(x)^2

static fp_t ann_activation_tanh(fp_t x) { return tanh(x); }
static fp_t ann_activation_tanh_partial(fp_t x) { return 1.0 - (x * x); }

// ann_error()
//
// Implementation of mean squared error for calculating the difference between
// the current and target outputs for the ann_t instance
//
// output - The current ann_t output
// target - The target ann_t output

fp_t ann_error(fp_t output, fp_t target) {
  fp_t error = output - target;
  error = 0.5 * error * error;

  return error;
}

// ann_error_partial()
//
// The partial derivative of mean squared error where n = 2
//
// output - The current ann_t output
// target - The target ann_t output

fp_t ann_error_partial(fp_t output, fp_t target) { return (output - target); }

// ann_error_total()
//
//
//
// output - The array of current outputs
// target - The array of target outputs
// n - The size of the output and target arrays

fp_t ann_error_total(fp_t const *output, fp_t const *target, uint_t n) {
  fp_t error = 0;
  for (uint_t i = 0; i < n; i++) {
    error += ann_error(output[i], target[i]);
  }

  return error;
}

// ann_set_activation()
//
// Set the activation functions for the hidden and output layers
//
// ann - The current ann_t instance
// activation_hidden - The hidden layer activation function
// activation_output - The output layer activation function

void ann_set_activation(ann_t *ann, ann_activation_t activation_hidden,
                        ann_activation_t activation_output) {
  ann->activation_hidden = ACTIVATION[activation_hidden][0];
  ann->activation_hidden_partial = ACTIVATION[activation_hidden][1];
  ann->activation_output = ACTIVATION[activation_output][0];
  ann->activation_output_partial = ACTIVATION[activation_output][1];
}

// ann_random()
//
// Set the weights of the ann_t instance to random numbers in the [-1, 1] range
//
// ann - The current ann_t instance

void ann_random(ann_t *ann) {
  uint_t l, j, i;
  fp_t *wb = ann->weight;
  for (l = 1; l < ann->layer_n; l++) {
    for (j = 0; j < ann->layer_neuron_n[l]; j++) {
      i = 0;
      for (; i < ann->layer_neuron_n[l - 1]; i++) {
        *wb++ = ann_random_range(-1.0, 1.0);
      }

      *wb++ = 0;
    }
  }
}

// ann_random_range()
//
// Remap a random value to a given range
//
// low - The lower bounds of the range
// high - The upper bounds of the range

static fp_t ann_random_range(fp_t low, fp_t high) {
  return (low + ((fp_t)rand()) * (high - low) / RAND_MAX);
}

// ann_print_neuron()
//
// Print a text representation of the neurons in a given ann_t instance
//
// ann - The current ann_t instance
// input - The input used for the ann_t propagation being printed
// output - The output used for the ann_t propagation being printed

void ann_print_neuron(ann_t *ann, fp_t const *const input,
                      fp_t const *const output) {
  fputs("-> ", stderr);
  for (uint_t i = 0; i < ann->layer_neuron_n[0]; i++) {
    fprintf(stderr, "  %+.*f", PRINT_PRECISION, input[i]);
  }

  fputs("\n", stderr);

  fp_t *n = ann->neuron;

  for (uint_t l = 1; l < ann->layer_n - 1; l++) {
    fprintf(stderr, "   ", l);
    for (uint_t i = 0; i < ann->layer_neuron_n[l]; i++) {
      fprintf(stderr, "  %+.*f", PRINT_PRECISION, *n);
      n++;
    }

    fputs("\n", stderr);
  }

  fputs("=  ", stderr);
  for (uint_t i = 0; i < ann->layer_neuron_n[ann->layer_n - 1]; i++) {
    fprintf(stderr, "  %+.*f", PRINT_PRECISION, output[i]);
  }

  fputs("\n\n", stderr);
}

// ann_print_weight()
//
// Print a text representation of weights in the given ann_t instance
//
// ann - The current ann_t instance

void ann_print_weight(ann_t *ann) {
  fp_t *weight = ann->weight;

  for (uint_t i = 1; i < ann->layer_n; i++) {
    for (uint_t j = 0; j < ann->layer_neuron_n[i - 1] + 1; j++) {
      for (uint_t k = 0; k < ann->layer_neuron_n[i]; k++) {
        fprintf(stderr, "  %+.*f", PRINT_PRECISION,
                *(weight + j + k * (ann->layer_neuron_n[i - 1] + 1)));
      }

      fputs("\n", stderr);
    }

    weight += ann->layer_neuron_n[i - 1] * ann->layer_neuron_n[i] +
              ann->layer_neuron_n[i];

    fputs("\n", stderr);
  }
}

#endif // ANN_IMPLEMENTATION