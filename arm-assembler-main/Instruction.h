#pragma once

#include <stdint.h>
#include <stdbool.h>

enum ID{
        //data processing
        lsl,
        add,
        addi,
        sub,
        subi,
        or,
        ori,
        cmp,
       
        // branching
        b,

        // indicates error
        invalid
};

enum COND {
    EQ,
    NE,
    GE,
    LT,
    GT,
    LE,
    AL
};

#define MAX_LABEL_LENGTH 64

struct Instruction {
    enum ID instruction_id;   // id indicating the instruction
    enum COND cond;
    uint8_t rd;            // register number destination
    uint8_t rn;            // register number source
    uint8_t rm;            // register number secondary source
    uint8_t rs;            // register number shift amount
    int32_t immediate;     // immediate, may use up to 32 bits 
    uint8_t shift_amount;  // shift amount 
    char label[MAX_LABEL_LENGTH]; // label of the line this Instruction appears on
    char branch_label[MAX_LABEL_LENGTH];  // label used by branch or jump instructions
};

/*
 * this: instruction to turn into a string
 * result: pointer to a character array; when function returns, array contains string representation of instruction
 */
void toString(struct Instruction this, char *result);

/*
 * returns true if a and b are equal, that is, all fields are equal
 */
bool equal(struct Instruction a, struct Instruction b);

/*
 * returns an instruction with default values for all fields
 */
struct Instruction newInstructionBlank();

/*
 * returns an instruction with the given field values
 */
struct Instruction newInstruction(enum ID instruction_id, enum COND cond, uint8_t rd, uint8_t rn, uint8_t rm, uint8_t rs, int32_t immediate, uint8_t shift_amount, char *label, char *branch_label);
