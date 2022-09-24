#pragma once
#include "Instruction.h"

/*
 * Returns an instruction that is like i, except it also has a label
 */
struct Instruction label(char* label, struct Instruction i) ;
/*
 * Returns an instruction that is like i, except it also has a cond
 */
struct Instruction cond(enum COND cond, struct Instruction i) ;

struct Instruction ADD(uint8_t rd, uint8_t rn, uint8_t rm) ;
struct Instruction ADDI(uint8_t rd, uint8_t rn, int32_t imm) ;
struct Instruction SUB(uint8_t rd, uint8_t rn, uint8_t rm) ;
struct Instruction SUBI(uint8_t rd, uint8_t rn, int32_t imm) ;
struct Instruction ORR(uint8_t rd, uint8_t rn, uint8_t rm) ;
struct Instruction ORRI(uint8_t rd, uint8_t rn, int32_t imm) ;
struct Instruction LSL(uint8_t rd, uint8_t rm, uint8_t shift_amount) ;
struct Instruction CMP(uint8_t rn, uint8_t rm) ;

/*
 * returns b instruction with the given branch_label
 * and 0 for immediate
 */
struct Instruction B(char* branch_label) ;
/*
 * returns b instruction with an empty string for branch_label
 * and imm for its immediate
 */
struct Instruction Bimm(int32_t imm) ;

/*
 * returns the invalid instruction
 */
struct Instruction INVALID();
