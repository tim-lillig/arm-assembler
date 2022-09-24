#include <string.h>
#include "InstructionFactory.h"

struct Instruction label(char* label, struct Instruction i) {
    strncpy(i.label, label, MAX_LABEL_LENGTH);
    return i;
}

struct Instruction cond(enum COND cond, struct Instruction i) {
    i.cond = cond;
    return i;
}

struct Instruction ADD(uint8_t rd, uint8_t rn, uint8_t rm) {
    return newInstruction(add, AL, rd, rn, rm, 0, 0, 0, "", "");
}

struct Instruction ADDI(uint8_t rd, uint8_t rn, int32_t imm) {
    return newInstruction(addi, AL, rd, rn, 0, 0, imm, 0, "", "");
}

struct Instruction SUB(uint8_t rd, uint8_t rn, uint8_t rm) {
    return newInstruction(sub, AL, rd, rn, rm, 0, 0, 0, "", "");
}

struct Instruction SUBI(uint8_t rd, uint8_t rn, int32_t imm) {
    return newInstruction(subi, AL, rd, rn, 0, 0, imm, 0, "", "");
}

struct Instruction ORR(uint8_t rd, uint8_t rn, uint8_t rm) {
    return newInstruction(or, AL, rd, rn, rm, 0, 0, 0, "", "");
}

struct Instruction ORRI(uint8_t rd, uint8_t rn, int32_t imm) {
    return newInstruction(ori, AL, rd, rn, 0, 0, imm, 0, "", "");
}

struct Instruction LSL(uint8_t rd, uint8_t rm, uint8_t shift_amount) {
    return newInstruction(lsl, AL, rd, 0, rm, 0, 0, shift_amount, "", "");
}

struct Instruction CMP(uint8_t rn, uint8_t rm) {
    return newInstruction(cmp, AL, 0, rn, rm, 0, 0, 0, "", "");
}

struct Instruction B(char* branch_label) {
    struct Instruction i = newInstruction(b, AL, 0, 0, 0, 0, 0, 0, "", "");
    strncpy(i.branch_label, branch_label, MAX_LABEL_LENGTH);
    return i;
}

struct Instruction Bimm(int32_t imm) {
    struct Instruction i = newInstruction(b, AL, 0, 0, 0, 0, imm, 0, "", "");
    return i;
}

struct Instruction INVALID() {
    return newInstruction(invalid, AL, 0, 0, 0, 0, 0, 0, "", "");
}