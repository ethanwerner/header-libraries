#define POOL_IMPLEMENTATION
#include "../include/pool.h"

int main() {
    pool_t*p = pool_init(10, 100);
    pool_print(p);
}