#pragma once
#include <stdint.h>

/* returns number resulting from rotating x left by n bits
* n must be < 32
*/
uint32_t rotl32c (uint32_t x, uint32_t n);