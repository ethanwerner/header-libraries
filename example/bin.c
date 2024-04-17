#include <stdint.h>
#include <stdio.h>

typedef int32_t bin_key_t;

#define TEST_LENGTH 10
#define TEST_START -2
#define TEST_STEP 0.5;

struct sample_t {
  bin_key_t time;
  double data;
};

#define BIN_IMPLEMENTATION
#include "../include/bin.h"

int main() {
  char const *path = "./test.bin";
  FILE *bin = bin_init(path, sizeof(struct sample_t));

  bin_key_t half = (bin_key_t)((TEST_START + TEST_LENGTH) / 2.0);

  // Insert half of the values using bin_insert()
  for (bin_key_t i = half - 1; i >= TEST_START; i--) {
    bin_insert(bin, 0, &(struct sample_t){.time = i, .data = 2.0 * i}, 1);
  }

  // Append half of the values to the end
  for (bin_key_t i = half; i < TEST_START + TEST_LENGTH; i++) {
    bin_append(bin, &(struct sample_t){.time = i, .data = 2.0 * i}, 1);
  }

  uint64_t l = bin_length(bin);
  uint64_t bs = bin_block_size(bin);

  struct sample_t x[l];
  bin_read(bin, 0, x, l);

  // Test that all values are correct
  for (int32_t i = 0; i < TEST_LENGTH; i++) {
    bin_key_t key = TEST_START + i;
    assert(x[i].time == key && x[i].data == 2.0 * key);
  }

  bin_close(bin);
}