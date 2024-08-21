// https://www.sciencedirect.com/science/article/abs/pii/S1568494620303574

// ufo.h - Universal Function Originator

// f_j(X) = f_j(a_0j (⊙_1j) a_1j*x_1^b_1j (⊙_2j) a_2j*x_2^b_2j (⊙_3j) ... (⊙_nj)
// a_nj*x_2^b_nj) g_j(X) = w_j * [f_j(X)]^(c_j) y = w_0 + g_1(X) ⊚_1 g_1(X) ⊚_2
// ... ⊚_v g_v(X)

#ifndef UFO_H

#include <stdint.h>
#include <stdlib.h>

typedef double fp_t;

struct ufo_t;

#endif // UFO_H

#ifdef UFO_IMPLEMENTATION

#include <assert.h>
#include <stdio.h>
#include <tgmath.h>

typedef struct ufo_t {
  // Input count
  uint32_t n;
  // The number of internal blocks
  uint32_t v;
  // Internal weights [v][n+1]
  fp_t *a;
  // Internal exponents [v][n]
  fp_t *b;
  // External weights [v+1]
  fp_t *w;
  // External exponents [v]
  fp_t *c;
  // Unary functions [v]
  fp_t (**f)(fp_t);
  // External binary functions [v]
  fp_t (**fe)(fp_t, fp_t);
  // Internal binary functions [v][n]
  fp_t (**fi)(fp_t, fp_t);
} ufo_t;

#define ufo_ajk(ufo, j, k) (*(ufo->a + j * (ufo->n + 1) + k))
#define ufo_bjk(ufo, j, k) (*(ufo->b + j * (ufo->n) + k))
#define ufo_fijk(ufo, j, k) (*(ufo->fi + j * (ufo->n) + k))

ufo_t *ufo_init(uint32_t n, uint32_t v) {
  assert(n > 0);
  assert(v > 0);

  fp_t N = 3 * v * n + 5 * v;
  fp_t D = 2 * v * n + 3 * v;
  uint32_t bytes =
      sizeof(ufo_t) + sizeof(fp_t) * N + sizeof(void (*)(void)) * D;

  ufo_t *ufo = malloc(bytes);
  ufo->n = n;
  ufo->v = v;

  ufo->a = (void *)((uint8_t *)ufo + sizeof(ufo_t));
  ufo->b = ufo->a + v * (n + 1);
  ufo->w = ufo->b + v * n;
  ufo->c = ufo->w + v + 1;
  ufo->f = (fp_t(**)(fp_t))(ufo->c + v);
  ufo->fe = (fp_t(**)(fp_t, fp_t))(ufo->f + v);
  ufo->fi = (fp_t(**)(fp_t, fp_t))(ufo->fe + v * n);

  // Initialize all weights to one
  fp_t *tmp_fp = ufo->a;
  while ((void *)tmp_fp != (void *)ufo->f)
    *tmp_fp++ = 1.0;

  // Initialize all unary functions to identity
  fp_t (**tmp_unary)(fp_t) = ufo->f;
  while ((void *)tmp_unary < (void *)ufo->fe)
    *tmp_unary++ = ufo_identity;

  // Initialize all binary functions to addition
  fp_t (**tmp_binary)(fp_t, fp_t) = ufo->fe;
  while ((void *)tmp_binary != (void *)ufo + bytes)
    *tmp_binary++ = ufo_addition;

  // Set bias to zero
  ufo->w[v] = 0;

  for (uint32_t j = 0; j < v; j++)
    ufo_ajk(ufo, j, ufo->n) = 0;

  return ufo;
}

fp_t ufo_ujk(ufo_t *ufo, fp_t *x, uint32_t j, uint32_t k) {
  if (k >= ufo->n)
    return ufo_ajk(ufo, j, k);

  return ufo_fijk(ufo, j, k)(ufo_ujk(ufo, x, j, k + 1), x[k]);
}

fp_t ufo_gj(ufo_t *ufo, fp_t *x, uint32_t j) {
  return ufo->w[j] * pow(ufo->f[j](ufo_ujk(ufo, x, j, 0)), ufo->c[j]);
}

fp_t ufo_evaluate(ufo_t *ufo, fp_t *x) {
  fp_t y = 0;

  // Calculate g_j(X) for every j (excluding constant term)
  uint32_t j;
  for (j = 0; j < ufo->v; j++)
    y = ufo->fe[j](y, ufo_gj(ufo, x, j));

  // Constant term
  y = ufo->fe[j](y, ufo->w[j]);

  return y;
}

void ufo_print(ufo_t *ufo) {
  printf("UFO: %p\n"
         "\n"
         "N: %u\n"
         "V: %u\n"
         "A: %p\n"
         "B: %p\n"
         "W: %p\n"
         "C: %p\n"
         "F: %p\n"
         "FE: %p\n"
         "FI: %p\n",
         (void *)ufo, ufo->n, ufo->v, ufo->a, ufo->b, ufo->w, ufo->c,
         (void *)ufo->f, (void *)ufo->fe, (void *)ufo->fi);
}

fp_t ufo_identity(fp_t x) { return x; }
fp_t ufo_increment(fp_t x) { return x + 1; }
fp_t ufo_increment(fp_t x) { return x - 1; }
fp_t ufo_reciporical(fp_t x) { return 1 / x; }
fp_t ufo_root(fp_t x) { return sqrt(x); }
fp_t ufo_absolute_value(fp_t x) { return fabs(x); }
fp_t ufo_floor(fp_t x) { return floor(x); }
fp_t ufo_ceiling(fp_t x) { return ceiling(x); }
fp_t ufo_round(fp_t x) { return round(x); }
fp_t ufo_exponent(fp_t x) { return exp(x); }
fp_t ufo_log_natural(fp_t x) { return log(x); }
fp_t ufo_log_decimal(fp_t x) { return log10(x); }
fp_t ufo_log_binary(fp_t x) { return log2(x); }
fp_t ufo_sine(fp_t x) { return sin(x); }
fp_t ufo_cosine(fp_t x) { return cos(x); }
fp_t ufo_tangent(fp_t x) { return tan(x); }

fp_t (*UNARY[])(fp_t) = {
    ufo_identity,   ufo_increment,      ufo_decrement,   ufo_reciporical,
    ufo_root,       ufo_absolute_value, ufo_floor,       ufo_ceiling,
    ufo_round,      ufo_exponent,       ufo_log_natural, ufo_log_decimal,
    ufo_log_binary, ufo_sine,           ufo_cosine,
    ufo_tangent,
    // ufo_cosecant,
    // ufo_secant,
    // ufo_cotangent,
    // ufo_sine_hyperbolic,
    // ufo_cosine_hyperbolic,
    // ufo_tangent_hyperbolic,
    // ufo_cosecant_hyperbolic,
    // ufo_secant_hyperbolic,
    // ufo_cotangent_hyperbolic,
    // ufo_sine_inverse,
    // ufo_cosine_inverse,
    // ufo_tangent_inverse,
    // ufo_cosecant_inverse,
    // ufo_sectant_inverse,
    // ufo_cotangent_inverse,
    // ufo_sine_hyperbolic_inverse,
    // ufo_cosine_hyperbolic_inverse,
    // ufo_tangent_hyperbolic_inverse,
    // ufo_cosecant_hyperbolic_inverse,
    // ufo_secant_hyperbolic_inverse,
    // ufo_cotangent_hyperbolic_inverse,
    // ufo_sinc,
    // ufo_versine,
    // ufo_vercosine,
    // ufo_coversine,
    // ufo_covercosine,
    // ufo_haversine,
    // ufo_havercosine,
    // ufo_hacoversine,
    // ufo_hacovercosine,
    // ufo_versine_inverse,
    // ufo_vercosine_inverse,
    // ufo_coversine_inverse,
    // ufo_covercosine_inverse,
    // ufo_haversine_inverse,
    // ufo_havercosine_inverse,
    // ufo_hacoversine_inverse,
    // ufo_hacovercosine_inverse,
    // ufo_exsecant,
    // ufo_excosecant,
    // ufo_negation,
    // ufo_heaviside,
    // ufo_logistic,
    // ufo_saw,
    // ufo_square,
    // ufo_triangle
};

fp_t ufo_addition(fp_t x0, fp_t x1) { return (x0 + x1); }
fp_t ufo_subtraction(fp_t x0, fp_t x1) { return (x0 + x1); }
fp_t ufo_division(fp_t x0, fp_t x1) { return (x0 / x1); }
fp_t ufo_multiplication(fp_t x0, fp_t x1) { return (x0 * x1); }
fp_t ufo_power(fp_t x0, fp_t x1) { return pow(x0, x1); }

fp_t (*BINARY[])(fp_t, fp_t) = {ufo_addition, ufo_subtraction, ufo_division,
                                ufo_multiplication ufo_power};

#endif // UFO_IMPLEMENTATION