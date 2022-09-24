#include <stdio.h>
#include <check.h>
#include <assert.h>
#include <stdlib.h>
#include "Instruction.h"
#include "InstructionFactory.h"
#include "ArrayList.h"
#include "Phase1.h"
#include "Phase2.h"
#include "Phase3.h"

const int TEXT_SEGMENT_START = 0x00400000;

void printInstructions(int n, struct Instruction ex, struct Instruction act) {
    if (!equal(ex, act)) {
      char exs[256];
      char acts[256];
      toString(ex, exs);
      toString(act, acts);
      printf("%d: %s == %s\n", n, exs, acts);
    }
}

void printMachineCodes(int n, uint32_t ex, uint32_t act) {
  if (ex != act) {
    printf("%d: %x == %x\n", n, ex, act);
  }
}

void testHelperPhase1(struct ArrayList *input, struct ArrayList *expectedP1) {
  // Phase 1
  struct ArrayList *checked = newArrayList();

  check_errors(input, checked);
  // same length?
  ck_assert(size(expectedP1) == size(checked));
  // same objects?
  for(int i = 0; i < size(expectedP1); i++){
    struct Instruction ex = get(expectedP1, i);
    struct Instruction act = get(checked, i);
    printInstructions(i, ex, act);

    ck_assert(equal(ex, act));
  }

  freeList(checked);
}
        
void testHelperPhase2(struct ArrayList *expectedP1, struct ArrayList *expectedP2) {
  // assertion not a test
  assert(size(expectedP1) == size(expectedP2));

  // Phase 2
  struct ArrayList *resolved_checked = newArrayList();

  resolve_addresses(expectedP1, TEXT_SEGMENT_START, resolved_checked);
  // same length?
  ck_assert(size(expectedP2) == size(resolved_checked));
  // same objects?
  for(int i = 0; i < size(expectedP2); i++){
    struct Instruction ex = get(expectedP2, i);
    struct Instruction act = get(resolved_checked, i);
    printInstructions(i, ex, act);

    ck_assert(equal(ex, act));
  }

  freeList(resolved_checked);
}
        
void testHelperPhase3(struct ArrayList *expectedP2, uint32_t expectedP3[]) {
  // Phase 3
  uint32_t *translated = malloc(size(expectedP2) * sizeof(uint32_t)); 

  translate_instructions(expectedP2, translated);
  // same objects?
  for(int i = 0; i < size(expectedP2); i++){
    printMachineCodes(i, expectedP3[i], translated[i]);
    ck_assert(expectedP3[i] == translated[i]);
  }
  
  free(translated);
}

START_TEST(test1Phase1) {
  struct Instruction input[5] = {
    // label1: add R8, R0, R0
    // add R12, R7, R4
    // cmp R12, R0
    // blt label1
    // add R13,R14,#0xF00000
    label("label1", ADD(8, 0, 0)),
    ADD(12, 7, 4),
    CMP(12, 0),
    cond(LT, B("label1")),
    ADDI(13, 14, 0xF00000)
  };
  // Phase 1
  struct Instruction phase1_expected[5] = {
    label("label1", ADD(8, 0, 0)),
    ADD(12, 7, 4),
    CMP(12, 0),
    cond(LT, B("label1")),
    ADDI(13, 14, 0xF00000)
  };
  struct ArrayList *inputL = fromArray(input, 5);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 5);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
}
END_TEST
        
START_TEST(test1Phase2) {
  // Phase 1
  struct Instruction phase1_expected[5] = {
    label("label1", ADD(8, 0, 0)),
    ADD(12, 7, 4),
    CMP(12, 0),
    cond(LT, B("label1")),
    ADDI(13, 14, 0xF00000)
  };

  // Phase 2
  struct Instruction phase2_expected[5] = {
    label("label1", ADD(8, 0, 0)),
    ADD(12, 7, 4),
    CMP(12, 0),
    cond(LT, Bimm(-5)),
    ADDI(13, 14, 0xF00000)
  };

  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 5);
  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 5);

  testHelperPhase2(phase1_expectedL, phase2_expectedL);

  freeList(phase1_expectedL);
  freeList(phase2_expectedL);
}
END_TEST
        
START_TEST(test1Phase3) {
  // Phase 2
  struct Instruction phase2_expected[5] = {
    label("label1", ADD(8, 0, 0)),
    ADD(12, 7, 4),
    CMP(12, 0),
    cond(LT, Bimm(-5)),
    ADDI(13, 14, 0xF00000)
  };

  // Phase 3
  uint32_t phase3_expected[5] = {
    // HINT: to get these, use ARMSim#
    0xE0808000,
    0xE087C004,
    0xE15C0000,
    0xBAFFFFFB,
    0xE28ED60F
  };

  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 5);
  testHelperPhase3(phase2_expectedL, phase3_expected);
  freeList(phase2_expectedL);
}
END_TEST
        
START_TEST(test2Phase1) { 
  struct Instruction input[5] = {
    // Label1: add R8, R3, #1
    // Label2: add R3, R7, #0x810000
    // Label3: cmp R3, R8
    // bgt Label2
    // Label4: blt Label1
    label("label1", ADDI(8, 3, 1)),
    label("label2", ADDI(3, 7, 0x810000)),
    label("label3", CMP(3, 8)),
    cond(GT, B("label2")),
    label("label4", cond(LT, B("label1")))
  };

  //Phase 1:
  struct Instruction phase1_expected[5] = {
    label("label1", ADDI(8, 3, 1)),
    label("label2", ADDI(3, 7, 0x810000)),
    label("label3", CMP(3, 8)),
    cond(GT, B("label2")),
    label("label4", cond(LT, B("label1")))
  };

  struct ArrayList *inputL = fromArray(input, 5);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 5);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
}
END_TEST
        
START_TEST(test2Phase2) {
  //Phase 1:
  struct Instruction phase1_expected[5] = {
    label("label1", ADDI(8, 3, 1)),
    label("label2", ADDI(3, 7, 0x810000)),
    label("label3", CMP(3, 8)),
    cond(GT, B("label2")),
    label("label4", cond(LT, B("label1")))
  };

  //Phase 2:
  struct Instruction phase2_expected[5] = {
    label("label1", ADDI(8, 3, 1)),
    label("label2", ADDI(3, 7, 0x810000)),
    label("label3", CMP(3, 8)),
    cond(GT, Bimm(-4)),
    label("label4", cond(LT, Bimm(-6)))
  };

  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 5);
  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 5);

  testHelperPhase2(phase1_expectedL, phase2_expectedL);

  freeList(phase1_expectedL);
  freeList(phase2_expectedL);
}
END_TEST
START_TEST(test2Phase3) {
  //Phase 2:
  struct Instruction phase2_expected[5] = {
    label("label1", ADDI(8, 3, 1)),
    label("label2", ADDI(3, 7, 0x810000)),
    label("label3", CMP(3, 8)),
    cond(GT, Bimm(-4)),
    label("label4", cond(LT, Bimm(-6)))
  };


  //Phase 3:   
  uint32_t phase3_expected[5] = {
    0xE2838001,
    0xE2873881,
    0xE1530008,
    0xCAFFFFFC,
    0xBAFFFFFA
  };

  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 5);
  testHelperPhase3(phase2_expectedL, phase3_expected);
  freeList(phase2_expectedL);
}
END_TEST
        
START_TEST(test3Phase1) {
    //add R12, R4, #0x520
    //add R12, R4, #0x1020
    //add R12, R4, #0x1040
    //add R12, R4, #0x1080
    //add R12, R4, #0x2080
    //add R12, R4, #0x4080
  struct Instruction input[7] = {
    ADDI(12, 4, 0x520),
    ADDI(12, 4, 0x1020),
    ADDI(12, 4, 0x1040),
    ADDI(12, 4, 0x1040),
    ADDI(12, 4, 0x1080),
    ADDI(12, 4, 0x2080),
    ADDI(12, 4, 0x4080),
  };

  // Phase 1
  struct Instruction phase1_expected[7] = {
    ADDI(12, 4, 0x520),
    INVALID(),
    ADDI(12, 4, 0x1040),
    ADDI(12, 4, 0x1040),
    ADDI(12, 4, 0x1080),
    ADDI(12, 4, 0x2080),
    INVALID(),
  };

  struct ArrayList *inputL = fromArray(input, 7);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 7);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
}
END_TEST

START_TEST(test4Phase1) {
    //add R14, R5, #0x7000000F
    //add R15, R6, #0x700000F0
    //add R1, R7,  #0x18000001
    //add R1, R7,  #0x02000001
  struct Instruction input[4] = {
    ADDI(14, 5, 0x7000000F),
    ADDI(15, 6, 0x700000F0),
    ADDI(1, 7,  0x18000001),
    ADDI(1, 7,  0x02000001)
  };

  // Phase 1
  struct Instruction phase1_expected[4] = {
    ADDI(14, 5, 0x7000000F),
    INVALID(),
    ADDI(1, 7,  0x18000001),
    INVALID()
  };

  struct ArrayList *inputL = fromArray(input, 4);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 4);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
} 
END_TEST

START_TEST(test5Phase1) {
    //add R11, R7, #-4
    //sub R11, R7, #-200
    //add R11, R7, #-400
    //sub R11, R7, #-25600
    //sub R11, R7, #-4128
    //sub R11, R7, #-4160 
  struct Instruction input[6] = {
    ADDI(11, 7, -4),
    SUBI(11, 7, -200),
    ADDI(11, 7, -400),
    SUBI(11, 7, -25600),
    SUBI(11, 7, -4128),
    SUBI(11, 7, -4160),
  };

  // Phase 1
  struct Instruction phase1_expected[6] = {
    SUBI(11, 7, 4),
    ADDI(11, 7, 200),
    SUBI(11, 7, 400),
    ADDI(11, 7, 25600),
    INVALID(),
    ADDI(11, 7, 4160),
  };

  struct ArrayList *inputL = fromArray(input, 6);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 6);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
}
END_TEST
        
        
START_TEST(test6Phase1) {
  struct Instruction input[2] = {
    // lsl R3, R4, #7
    // label7: lsl R4, R5, #30
    LSL(3, 4, 7),
    label("label7", LSL(4, 5, 30))
  };
  // Phase 1
  struct Instruction phase1_expected[2] = {
    LSL(3, 4, 7),
    label("label7", LSL(4, 5, 30))
  };

  struct ArrayList *inputL = fromArray(input, 2);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 2);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
}
END_TEST
        
START_TEST(test6Phase2) {
  // Phase 1
  struct Instruction phase1_expected[2] = {
    LSL(3, 4, 7),
    label("label7", LSL(4, 5, 30))
  };

  // Phase 2
  struct Instruction phase2_expected[2] = {
    LSL(3, 4, 7),
    label("label7", LSL(4, 5, 30))
  };

  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 2);
  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 2);

  testHelperPhase2(phase1_expectedL, phase2_expectedL);

  freeList(phase1_expectedL);
  freeList(phase2_expectedL);
}
END_TEST
        
START_TEST(test6Phase3) {
  // Phase 2
  struct Instruction phase2_expected[2] = {
    LSL(3, 4, 7),
    label("label7", LSL(4, 5, 30))
  };

  // Phase 3
  uint32_t phase3_expected[2] = {
    0xE1A03384,
    0xE1A04F05
  };

  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 2);
  testHelperPhase3(phase2_expectedL, phase3_expected);
  freeList(phase2_expectedL);
}
END_TEST
        
START_TEST(test7Phase1) {
  struct Instruction input[4] = {
    // orr R6, R8, #0x66
    // orr R6, R8, #-4
    // orr R6, R8, #0xC180
    // orr R6, R8, #0x78000000
    ORRI(6, 8, 0x66),
    ORRI(6, 8, -4),
    ORRI(6, 8, 0xC180),
    ORRI(6, 8, 0x78000000)
  };
  // Phase 1
  struct Instruction phase1_expected[4] = {
    ORRI(6, 8, 0x66),
    INVALID(),
    INVALID(),
    ORRI(6, 8, 0x78000000)
  };

  struct ArrayList *inputL = fromArray(input, 4);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 4);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
}
END_TEST
        
START_TEST(test8Phase1) {
  // addeq R1, R2, R3
  // label0: sublt R4, R5, R6
  // orrle R7, R8, R9
  // lslne R10, R11, #12
  struct Instruction input[4] = {
    cond(EQ, ADD(1, 2, 3)),
    label("label0", cond(LT, SUB(4, 5, 6))),
    cond(LE, ORR(7, 8, 9)),
    cond(NE, LSL(10, 11, 12))
  };

  // Phase 1
  struct Instruction phase1_expected[4] = {
    cond(EQ, ADD(1, 2, 3)),
    label("label0", cond(LT, SUB(4, 5, 6))),
    cond(LE, ORR(7, 8, 9)),
    cond(NE, LSL(10, 11, 12))
  };

  struct ArrayList *inputL = fromArray(input, 4);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 4);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
}
END_TEST
        
START_TEST(test8Phase2) {
  // Phase 1
  struct Instruction phase1_expected[4] = {
    cond(EQ, ADD(1, 2, 3)),
    label("label0", cond(LT, SUB(4, 5, 6))),
    cond(LE, ORR(7, 8, 9)),
    cond(NE, LSL(10, 11, 12))
  };

  // Phase 2
  struct Instruction phase2_expected[4] = {
    cond(EQ, ADD(1, 2, 3)),
    label("label0", cond(LT, SUB(4, 5, 6))),
    cond(LE, ORR(7, 8, 9)),
    cond(NE, LSL(10, 11, 12))
  };

  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 4);
  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 4);

  testHelperPhase2(phase1_expectedL, phase2_expectedL);

  freeList(phase1_expectedL);
  freeList(phase2_expectedL);
}
END_TEST
        
START_TEST(test8Phase3) {
  // Phase 2
  struct Instruction phase2_expected[4] = {
    cond(EQ, ADD(1, 2, 3)),
    label("label0", cond(LT, SUB(4, 5, 6))),
    cond(LE, ORR(7, 8, 9)),
    cond(NE, LSL(10, 11, 12))
  };

  // Phase 3
  uint32_t phase3_expected[4] = {
    0x00821003,
    0xB0454006,
    0xD1887009,
    0x11A0A60B
  }; 

  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 4);
  testHelperPhase3(phase2_expectedL, phase3_expected);
  freeList(phase2_expectedL);
}
END_TEST
        
START_TEST(test9Phase1) {
  struct Instruction input[8] = {
      // sub R0, R0, R0
      // add R1, R0, #-100
      // loop: cmp R0, R1
      // beq end
      // add R0, R0, #-1
      // lsl R2, R2, #4
      // b loop
      // end: add R3, R2, #0
      SUB(0, 0, 0),
      ADDI(1, 0, -100),
      label("loop", CMP(0, 1)),
      cond(EQ, B("end")),
      ADDI(0, 0, -1),
      LSL(2, 2, 4),
      B("loop"),
      label("end", ADDI(3, 2, 0))
  };
  // Phase 1
  struct Instruction phase1_expected[8] = {
      SUB(0, 0, 0),
      SUBI(1, 0, 100),
      label("loop", CMP(0, 1)),
      cond(EQ, B("end")),
      SUBI(0, 0, 1),
      LSL(2, 2, 4),
      B("loop"),
      label("end", ADDI(3, 2, 0))
  };

  struct ArrayList *inputL = fromArray(input, 8);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 8);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
}
END_TEST
        
START_TEST(test9Phase2) {
  // Phase 1
  struct Instruction phase1_expected[8] = {
      SUB(0, 0, 0),
      SUBI(1, 0, 100),
      label("loop", CMP(0, 1)),
      cond(EQ, B("end")),
      SUBI(0, 0, 1),
      LSL(2, 2, 4),
      B("loop"),
      label("end", ADDI(3, 2, 0))
  };

  // Phase 2
  struct Instruction phase2_expected[8] = {
      SUB(0, 0, 0),
      SUBI(1, 0, 100),
      label("loop", CMP(0, 1)),
      cond(EQ, Bimm(2)),
      SUBI(0, 0, 1),
      LSL(2, 2, 4),
      Bimm(-6),
      label("end", ADDI(3, 2, 0))
  };

  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 8);
  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 8);

  testHelperPhase2(phase1_expectedL, phase2_expectedL);

  freeList(phase1_expectedL);
  freeList(phase2_expectedL);
}
END_TEST
        
START_TEST(test9Phase3) {
  // Phase 2
  struct Instruction phase2_expected[8] = {
      SUB(0, 0, 0),
      SUBI(1, 0, 100),
      label("loop", CMP(0, 1)),
      cond(EQ, Bimm(2)),
      SUBI(0, 0, 1),
      LSL(2, 2, 4),
      Bimm(-6),
      label("end", ADDI(3, 2, 0))
  };

  // Phase 3
  uint32_t phase3_expected[8] = {
    0xE0400000,
    0xE2401064,
    0xE1500001,
    0x0A000002,
    0xE2400001,
    0xE1A02202,
    0xEAFFFFFA,
    0xE2823000
  };

  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 8);
  testHelperPhase3(phase2_expectedL, phase3_expected);
  freeList(phase2_expectedL);
}
END_TEST
        
START_TEST(test10Phase1) {
  struct Instruction input[6] = {
    //a: b a
    //b: bne f 
    //c: blt f
    //d: blt c
    //e: bge f
    //f: b d
    label("a", B("a")),
    label("b", cond(NE, B("f"))),
    label("c", cond(LT, B("f"))),
    label("d", cond(LT, B("c"))),
    label("e", cond(GE, B("f"))),
    label("f", B("d"))
  };
  // Phase 1
  struct Instruction phase1_expected[6] = {
    label("a", B("a")),
    label("b", cond(NE, B("f"))),
    label("c", cond(LT, B("f"))),
    label("d", cond(LT, B("c"))),
    label("e", cond(GE, B("f"))),
    label("f", B("d"))
  };

  struct ArrayList *inputL = fromArray(input, 6);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 6);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
}
END_TEST
        
START_TEST(test10Phase2) {
  // Phase 1
  // Phase 1
  struct Instruction phase1_expected[6] = {
    label("a", B("a")),
    label("b", cond(NE, B("f"))),
    label("c", cond(LT, B("f"))),
    label("d", cond(LT, B("c"))),
    label("e", cond(GE, B("f"))),
    label("f", B("d"))
  };

  // Phase 2
  struct Instruction phase2_expected[6] = {
    label("a", Bimm(-2)),
    label("b", cond(NE, Bimm(2))),
    label("c", cond(LT, Bimm(1))),
    label("d", cond(LT, Bimm(-3))),
    label("e", cond(GE, Bimm(-1))),
    label("f", Bimm(-4))
  };

  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 6);
  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 6);

  testHelperPhase2(phase1_expectedL, phase2_expectedL);

  freeList(phase1_expectedL);
  freeList(phase2_expectedL);
}
END_TEST
        
START_TEST(test10Phase3) {
  // Phase 2
  struct Instruction phase2_expected[6] = {
    label("a", Bimm(-2)),
    label("b", cond(NE, Bimm(2))),
    label("c", cond(LT, Bimm(1))),
    label("d", cond(LT, Bimm(-3))),
    label("e", cond(GE, Bimm(-1))),
    label("f", Bimm(-4))
  };

  // Phase 3
  uint32_t phase3_expected[6] = {
    0xEAFFFFFE,
    0x1A000002,
    0xBA000001,
    0xBAFFFFFD,
    0xAAFFFFFF,
    0xEAFFFFFC
  };

  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 6);
  testHelperPhase3(phase2_expectedL, phase3_expected);
  freeList(phase2_expectedL);
}
END_TEST

START_TEST(test11Phase1) {
  struct Instruction input[5] = {
    // orr R6, R8, #0xE0000002
    // orr R6, R8, #0x70000001
    // orr R6, R8, #0x00056000
    // add R6, R8, #0x80000003
    // sub R6, R8, #0x80000004
    ORRI(6, 8, 0xE0000002),
    ORRI(6, 8, 0x70000001),
    ORRI(6, 8, 0x00056000),
    ADDI(6, 8, 0x80000003),
    SUBI(6, 8, 0x80000004)
  };
  // Phase 1
  struct Instruction phase1_expected[5] = {
    ORRI(6, 8, 0xE0000002),
    ORRI(6, 8, 0x70000001),
    ORRI(6, 8, 0x00056000),
    ADDI(6, 8, 0x80000003),
    SUBI(6, 8, 0x80000004)
  };

  struct ArrayList *inputL = fromArray(input, 5);
  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 5);

  testHelperPhase1(inputL, phase1_expectedL);

  freeList(inputL);
  freeList(phase1_expectedL);
}
END_TEST
        
START_TEST(test11Phase2) {
  // Phase 1
  struct Instruction phase1_expected[5] = {
    ORRI(6, 8, 0xE0000002),
    ORRI(6, 8, 0x70000001),
    ORRI(6, 8, 0x00056000),
    ADDI(6, 8, 0xC0000003),
    SUBI(6, 8, 0x80000004)
  };

  // Phase 2
  struct Instruction phase2_expected[5] = {
    ORRI(6, 8, 0xE0000002),
    ORRI(6, 8, 0x70000001),
    ORRI(6, 8, 0x00056000),
    ADDI(6, 8, 0xC0000003),
    SUBI(6, 8, 0x80000004)
  };

  struct ArrayList *phase1_expectedL = fromArray(phase1_expected, 5);
  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 5);

  testHelperPhase2(phase1_expectedL, phase2_expectedL);

  freeList(phase1_expectedL);
  freeList(phase2_expectedL);
}
END_TEST

START_TEST(test11Phase3) {
  // Phase 2
  struct Instruction phase2_expected[5] = {
    ORRI(6, 8, 0xE0000002),
    ORRI(6, 8, 0x70000001),
    ORRI(6, 8, 0x00056000),
    ADDI(6, 8, 0xC0000003),
    SUBI(6, 8, 0x80000004)
  };

  // Phase 3
  uint32_t phase3_expected[5] = {
      0xE388622E,
      0xE3886217,
      0xE3886A56,
      0xE288610F,
      0xE2486112
  };

  struct ArrayList *phase2_expectedL = fromArray(phase2_expected, 5);
  testHelperPhase3(phase2_expectedL, phase3_expected);
  freeList(phase2_expectedL);
}
END_TEST

Suite * phase1_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Phase 1");

    // Core test case 
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test1Phase1);
    tcase_add_test(tc_core, test2Phase1);
    tcase_add_test(tc_core, test3Phase1);
    tcase_add_test(tc_core, test4Phase1);
    tcase_add_test(tc_core, test5Phase1);
    tcase_add_test(tc_core, test6Phase1);
    tcase_add_test(tc_core, test7Phase1);
    tcase_add_test(tc_core, test8Phase1);
    tcase_add_test(tc_core, test9Phase1);
    tcase_add_test(tc_core, test10Phase1);
    tcase_add_test(tc_core, test11Phase1);

    suite_add_tcase(s, tc_core);

    return s;
}

Suite * phase2_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Phase 2");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test1Phase2);
    tcase_add_test(tc_core, test2Phase2);
    tcase_add_test(tc_core, test6Phase2);
    tcase_add_test(tc_core, test8Phase2);
    tcase_add_test(tc_core, test9Phase2);
    tcase_add_test(tc_core, test10Phase2);
    tcase_add_test(tc_core, test11Phase2);

    suite_add_tcase(s, tc_core);

    return s;
}

Suite * phase3_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Phase 3");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test1Phase3);
    tcase_add_test(tc_core, test2Phase3);
    tcase_add_test(tc_core, test6Phase3);
    tcase_add_test(tc_core, test8Phase3);
    tcase_add_test(tc_core, test9Phase3);
    tcase_add_test(tc_core, test10Phase3);
    tcase_add_test(tc_core, test11Phase3);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    // Phase 1
    {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = phase1_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    printf("Phase 1: # of failed: %d\n", number_failed);
    }
    // Phase 2
    {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = phase2_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    printf("Phase 2: # of failed: %d\n", number_failed);
    }
    // Phase 3
    {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = phase3_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    printf("Phase 3: # of failed: %d\n", number_failed);
    }
}

