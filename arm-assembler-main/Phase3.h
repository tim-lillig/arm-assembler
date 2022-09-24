#pragma once
#include "Instruction.h"
#include "ArrayList.h"

/* Translate each Instruction object into
 * a 32-bit number.
 *
 * complete: list of Instructions to translate
 * machineCode: an array that is at least as long as the number of elements in complete; 
 *              after the function returns, contains the instructions in 32-bit binary representation
 * 
 */
void translate_instructions(struct ArrayList *complete, uint32_t machineCode[]);
