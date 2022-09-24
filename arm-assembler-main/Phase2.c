#include "Phase2.h"
#include "ArrayList.h"
#include <stdlib.h>
#include <stdio.h>
#include "InstructionFactory.h"
#include <string.h>

//resolves the immediate field with adresses from branch_label and returns list of instructions
void resolve_addresses(struct ArrayList *unresolved, uint32_t first_pc, struct ArrayList *resolved) {
    uint32_t pc = first_pc;
    uint32_t innerpc = first_pc;
    for (int i=0; i<unresolved->size; i++){
        innerpc = first_pc;
        for(int j=0; j<unresolved->size; j++){
            if (strcmp(unresolved->elements[j].branch_label, unresolved->elements[i].label) == 0 && strcmp(unresolved->elements[i].label, "") != 0){
                
                int temp = (pc - innerpc);
                temp -= 8;
                temp /= 4;
                unresolved->elements[j].immediate = temp;
                strcpy(unresolved->elements[j].branch_label, "");
                
            }
                innerpc += 4;
            }
        pc += 4;
    }
    for (int i=0; i<unresolved->size; i++){
        addLast(resolved, unresolved->elements[i]);
    }
}

