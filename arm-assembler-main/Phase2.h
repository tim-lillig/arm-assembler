#pragma once
#include <stdint.h>
#include "Instruction.h"
#include "ArrayList.h"

/* Returns a list of copies of the Instructions with the
 * immediate field of the instruction filled in
 * with the address calculated from the branch_label. And the branch_label turned to empty string.
 *
 * The instruction should not be changed if it is not a branch instruction
 *
 * unresolved: input program as a list, whose branch instructions don't have resolved immediate
 * first_pc: address where the first instruction of the program will eventually be placed in memory
 * resolved: an empty list; after the function returns, resolved contains the resulting instructions
 */
void resolve_addresses(struct ArrayList *unresolved, uint32_t first_pc, struct ArrayList *resolved);
