#include "Phase3.h"
#include "ArrayList.h"
#include <stdlib.h>
#include <stdio.h>


//returns 8bit rotated immediate
uint8_t Rotate(int32_t immediate) {
  for(int i=0; i<32; i += 2){
    int rotated = rotl32c(immediate, i);
    if(rotated < 256 && rotated > -256){
        return rotated;
    }
  }
    return;
}
//returns rotate value of immediate
uint8_t RotateAmount(int32_t immediate) {
  for(int i=0; i<32; i += 2){
    int rotated = rotl32c(immediate, i);
    if(rotated < 256 && rotated > -256){
        return i;
    }
  }
  return;
}

//translates instructions to machine code
void translate_instructions(struct ArrayList *complete, uint32_t machineCode[]) {
    uint32_t condition;
    uint32_t cmd;
    uint32_t opcode;
    uint32_t s;
    uint32_t icmd;
    uint8_t imm8;
    uint8_t rot;
    for(int i=0; i<complete->size; i++){
        if (complete->elements[i].immediate > 255 && complete->elements[i].immediate != 0){
            imm8 = Rotate(complete->elements[i].immediate);
            rot = RotateAmount(complete->elements[i].immediate);
            complete->elements[i].immediate = imm8;
        }
        
        //cond
        if(complete->elements[i].cond == EQ){
            condition = 0000;
        }
        else if(complete->elements[i].cond == NE){
            condition = 0001;
        }
        else if(complete->elements[i].cond == GE){
            condition = 1010;
        }
        else if(complete->elements[i].cond == LT){
            condition = 1011;
        }
        else if(complete->elements[i].cond == GT){
            condition = 1100;
        }
        else if(complete->elements[i].cond == LE){
            condition = 1101;
        }
        else if(complete->elements[i].cond == AL){
            condition = 1110;
        }
        
        //op
        opcode = 00;

         //i
        if(complete->elements[i].rm != 0){
            icmd = 0;
        }
        else{
            icmd = 1;
        }
        
        //cmd/s
        if (complete->elements[i].instruction_id == lsl){
            cmd = 1101;
            s=0;
            
        }
        else if (complete->elements[i].instruction_id == add){
            cmd = 0100;
            s=0;
        }
        else if (complete->elements[i].instruction_id == addi){
            cmd = 0100;
            s=0;
        }
        else if (complete->elements[i].instruction_id == sub){
            cmd = 0010;
            s=0;
        }
        else if (complete->elements[i].instruction_id == subi){
            cmd = 0010;
            s=0;
        }
        else if (complete->elements[i].instruction_id == or){
            cmd = 1100;
            s=0;
        }
        else if (complete->elements[i].instruction_id == ori){
            cmd = 1100;
            s=0;
        }
        else if (complete->elements[i].instruction_id == cmp){
            cmd = 1010;
            s = 1;
        }
        else if (complete->elements[i].instruction_id == b){
            cmd = 0111;
            s=0;
        }
        else{
            cmd = 0;
            s=0;
        }
    }
    
}
        
