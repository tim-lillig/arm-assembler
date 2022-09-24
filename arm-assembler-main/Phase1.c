#include "Phase1.h"
#include "ArrayList.h"
#include <string.h>
#include <stdio.h>
#include "rotate.h"
#include "InstructionFactory.h"

//check if immediate is rotatable into 8 bits
bool isRotatable(int32_t immediate) {
  for(int i=0; i<32; i += 2){
    int rotated = rotl32c(immediate, i);
    if(rotated < 256 && rotated > -256){
      return true;
    }
  }
  return false;
}


//checks if instructions are valid & modifies if necessary
void check_errors(struct ArrayList *unchecked, struct ArrayList *checked) {
    for(int i=0; i<unchecked->size; i++){
        if(unchecked->elements[i].immediate == 0){
            addLast(checked, unchecked->elements[i]);
        }
        else if(unchecked->elements[i].instruction_id == subi && unchecked->elements[i].immediate < 0){
            if(isRotatable(unchecked->elements[i].immediate*-1)){
              struct Instruction temp = ADDI(unchecked->elements[i].rd, unchecked->elements[i].rn, unchecked->elements[i].immediate * -1);
              addLast(checked, temp);
            }
            else{
              struct Instruction temp = INVALID();
              addLast(checked, temp);
            }
        }
        else if(unchecked->elements[i].instruction_id == addi && unchecked->elements[i].immediate < 0){
          if(isRotatable(unchecked->elements[i].immediate*-1)){
            struct Instruction temp = SUBI(unchecked->elements[i].rd, unchecked->elements[i].rn, unchecked->elements[i].immediate * -1);
            addLast(checked, temp);
          }
          else{
            struct Instruction temp = INVALID();
            addLast(checked, temp);
          }
        }
        else if(isRotatable(unchecked->elements[i].immediate)){
            addLast(checked, unchecked->elements[i]);
        }
        else{
            struct Instruction temp = INVALID();
            addLast(checked, temp);
        }
            
        
    }
    
}
