#pragma once
#include "ArrayList.h"

/* Checks if each instruction has a valid constant 
 *         (representable as imm8 with an even right rotation)
 * Will change an ADDI to SUBI (or vice versa) if negating the constant
 * would allow it to achieve the above constraint 
 *
 * unchecked: list of instructions that need to get checked
 * checked: an empty list. When the function returns it will be full of 
 *      copies of okay instructions, adds turned to subs and vice versa when needed,
 *      and invalid instructions replacing instructions that had errors.
 */     
void check_errors(struct ArrayList *unchecked, struct ArrayList *checked);
