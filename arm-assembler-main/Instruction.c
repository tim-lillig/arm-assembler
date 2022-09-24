#include "AssemblerException.h"
#include "Instruction.h"
#include <stdio.h>
#include <string.h>

    
struct Instruction newInstructionBlank() {
  return newInstruction(add, AL, 0, 0, 0, 0, 0, 0, "", "");
}

struct Instruction newInstruction(enum ID instruction_id, enum COND cond, uint8_t rd, uint8_t rn, uint8_t rm, uint8_t rs, int32_t immediate, uint8_t shift_amount, char *label, char *branch_label) {
  if (rd >= 16 || rd < 0) assemblerException("Unrecognized register: %d", rd);
  if (rn >= 16 || rn < 0) assemblerException("Unrecognized register: %d", rs);
  if (rm >= 16 || rm < 0) assemblerException("Unrecognized register: %d", rm);
  if (rs >= 16 || rs < 0) assemblerException("Unrecognized register: %d", rs);
  if (shift_amount >= 32 || shift_amount < 0) assemblerException("shift amount out of range: %d", shift_amount);

  struct Instruction r;
  r.instruction_id = instruction_id;
  r.cond = cond;
  r.rd = rd;
  r.rm = rm;
  r.rn = rn;
  r.rs = rs;
  r.immediate = immediate;
  r.shift_amount = shift_amount;
  strncpy(r.label, label, MAX_LABEL_LENGTH);
  strncpy(r.branch_label, branch_label, MAX_LABEL_LENGTH);

  return r;
}

bool equal(struct Instruction a, struct Instruction b) {
  return a.instruction_id == b.instruction_id
      && a.cond == b.cond
      && a.rd == b.rd
      && a.rn == b.rn
      && a.rm == b.rm
      && a.rs == b.rs
      && a.immediate == b.immediate
      && a.shift_amount == b.shift_amount
      && strncmp(a.label, b.label, MAX_LABEL_LENGTH) == 0
      && strncmp(a.label, b.label, MAX_LABEL_LENGTH) == 0;
}
    
/*
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Instruction that = (Instruction) o;

        if (instruction_id != that.instruction_id) return false;
        if (rd != that.rd) return false;
        if (rs != that.rs) return false;
        if (rt != that.rt) return false;
        if (immediate != that.immediate) return false;
        if (jump_address != that.jump_address) return false;
        if (shift_amount != that.shift_amount) return false;
        if (label == null ? that.label != null : !label.equals(that.label)) return false;
        return (branch_label == null ? that.branch_label == null : branch_label.equals(that.branch_label));

    }

    @Override
    public int hashCode() {
        int result = instruction_id.hashCode();
        result = 31 * result + rd;
        result = 31 * result + rs;
        result = 31 * result + rt;
        result = 31 * result + immediate;
        result = 31 * result + jump_address;
        result = 31 * result + shift_amount;
        result = 31 * result + label.hashCode();
        result = 31 * result + branch_label.hashCode();
        return result;
    }
*/

void toString(struct Instruction this, char *result) {
  sprintf(result, "Instruction{instruction_id=%u"
      ", cond=%u"
      ", rd=%u"
      ", rn=%u"
      ", rm=%u"
      ", rs=%u"
      ", immediate=%d"
      ", shift_amount=%u"
      ", label_id=%s"
      ", branch_label=%s"
      "}", 
      this.instruction_id,
      this.cond,
      this.rd,
      this.rn,
      this.rm,
      this.rs,
      this.immediate,
      this.shift_amount,
      this.label,
      this.branch_label);
}
