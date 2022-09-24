#include "rotate.h"
#include <assert.h>

//https://blog.regehr.org/archives/1063
uint32_t rotl32c (uint32_t x, uint32_t n) {
    assert (n < 32);
    return (x<<n) | (x>>(-n&31));
}