#ifndef __bitwise_h
#define __bitwise_h

#define INSTRUCTION_A_FIELD             15:0
#define INSTRUCTION_JMP                 2:0
#define INSTRUCTION_DST                 5:3
#define INSTRUCTION_EXPR                12:6
#define INSTRUCTION_C_FIELD             15:13
#define C_FILED                         7

#define LOW(f)                  (0?f)
#define HIGH(f)                 (1?f)

#define SHIFT_MASK(f)           (0xffff >> (16 - (HIGH(f)-LOW(f)+1)))
#define SET_MASK(f)             (SHIFT_MASK(f) << LOW(f))
#define VAL_MASK(v,f)           (v & SET_MASK(f))
#define SET_VAL(v,f)            ((v & SHIFT_MASK(f))<<LOW(f))
#define SET_FIELD(v,n,f)        (v | SET_VAL(n,f))
#define GET_VAL(str)            ((unsigned short)strtoul(str,NULL, 0))
#endif