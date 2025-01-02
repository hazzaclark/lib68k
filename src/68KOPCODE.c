/* COPYRIHGT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE ACTS AS AN EXPANSION OF THE PRE-REQUISTIES DEFINED IN THE MAIN CPU HEADER */
/* PROVIDING MOODULARISATION AND CROSS COMPILATION FOR OPCODE RELATED HANDLERS */

/* NESTED INCLUDES */

#include "68K.h"

#ifdef BUILD_OP_TABLE

OPCODE OPCODE_BASE;
OPCODE* FIND_OPCODE(char* NAME, int SIZE);
void(*M68K_OPCODE_JUMP_TABLE[0x10000])(void);
static int CYCLE_INDEX[OPCODE_MAX];

/* EXCEPTION HANDLER FOR A-LINE INSTRUCTION HANDLERS */
/* DISCERN THE CURRENT EXCEPTION BEING FED INTO PC AND APPEND THAT THROUGH A JMP */ 

void M68K_OP_1010(void)
{
    M68K_SET_FUNC_CALLBACK(M68K_REG_PC);
    M68K_JUMP_VECTOR(M68K_EXCEPTION_1010);

    /* USE THE INTEGREAL POINTER NOTATION TO ACCESS THE EXCEPTION FROM THE CURRENT A-LINE INSTRUCTION */
    /* PROVIDE ACCESS TO THE INDEX REGISTER */

    M68K_USE_CYCLES(M68K_CYCLE[M68K_EXCEPTION_1010]);
    M68K_GET_CYCLES() = M68K_CYCLE[M68K_REG_IR];
}

/* SAME THING BUT FOR F-LINE INSTRUCTION HANDLERS */

void M68K_OP_1111(void)
{
    M68K_SET_FUNC_CALLBACK(M68K_REG_PC);
    M68K_JUMP_VECTOR(M68K_EXCEPTION_1111);

    M68K_USE_CYCLES(M68K_CYC_EXCE[M68K_EXCEPTION_1111]);
    M68K_GET_CYCLES() = M68K_CYC_EXCE[M68K_REG_IR];

}

/* THE FOLLOWING WILL REFER TO THE BITWISE MEANS BY WHICH ALL OF THESE OPCODES OPERATE */
/* ALL OF THE SUBSEQUENT INFORMATION CAN BE SOURED FROM: https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf */

/* FORTUNTATELY, THERE IS A LOT OF INFORMATION PERTAINING TOWARDS HOW EVERYTHING WORKS IN THE DESC */
/* THERE IS A LOT OF NUANCE WITH THESE INSTRUCTIONS - SO THE ORDER OF OPERATIONS WILL VARY */

/* THE GENERAL JYST IS WORKING WITH POINTERS BETWEEN A SOURCE AND A DESTINATION */

/* SEE INTEGER INSTRUCTIONS: https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf#PAGE=105 */

M68K_MAKE_OPCODE(ABCD, 8, RR, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = M68K_DATA_LOW;

    /* THERE *MUST* BE AN EASIER WAY OF DEFINING THE POSITION AND LOCATION OF OPERANDS */
    /* THIS IS VERY SCUFFED AT THE MOMENT, SO BARE WITH ME AS I IRON THIS OUT, THIS DOES NOT SEEM RIGHT */

    /* WHAT THIS IS DOING IS READING THE LOW DATA SECTION WITH THE SRC OPERAND, DESTINATION BASED OF THE EXTEND FLAG */
    /* THE EXTEND FLAG GOVERNS THE BASIS OF SETTING THE SAME BIT CURRENTLY IN THE OPERATION TO THE CARRY BIT (CARRY TO DEST) */

    int RESULT = M68K_LOW_NIBBLE(SRC) + M68K_LOW_NIBBLE(DESTINATION) + M68K_FLAG_X >> DESTINATION;

    /* IF THE DATA IS OUT OF THE LOW NIBBLE RANGE (0 - 8) */

    if(RESULT > 9)
    {
        RESULT += M68K_HIGH_NIBBLE(SRC) + M68K_HIGH_NIBBLE(DESTINATION);
    }

    M68K_FLAG_V += M68K_HIGH_NIBBLE(SRC) + M68K_HIGH_NIBBLE(DESTINATION);
    DESTINATION = M68K_MASK_OUT_ABOVE_8(DESTINATION) | RESULT;
}   


M68K_MAKE_OPCODE(ADD, 8, EA, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = M68K_MASK_OUT_ABOVE_8(M68K_DATA_LOW);
    int RESULT = SRC + DESTINATION;

    M68K_FLAG_N = (U8)RESULT;
    M68K_FLAG_V = ((U8)(SRC + DESTINATION + RESULT));
    M68K_FLAG_X = M68K_FLAG_C = (U8)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_8(RESULT);

    RESULT = M68K_MASK_OUT_ABOVE_8(RESULT) | M68K_FLAG_Z;
}

M68K_MAKE_OPCODE(ADD, 16, EA, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = M68K_MASK_OUT_ABOVE_16(M68K_DATA_LOW);

    int RESULT = SRC + DESTINATION;

    M68K_FLAG_N = (U16)RESULT;
    M68K_FLAG_V = ((U16)(SRC + DESTINATION + RESULT));
    M68K_FLAG_X = M68K_FLAG_C = (U16)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_16(RESULT);

    RESULT = M68K_MASK_OUT_ABOVE_16(RESULT) | M68K_FLAG_Z;
}

M68K_MAKE_OPCODE(ADD, 32, EA, 0)
{
    int DESTINATIOON = M68K_DATA_HIGH;
    int SRC = M68K_ADDRESS_LOW;

    int RESULT = SRC + DESTINATIOON;

    M68K_FLAG_N = (U32)RESULT;
    M68K_FLAG_V = ((U32)(SRC + DESTINATIOON + RESULT));

    M68K_FLAG_X = M68K_FLAG_C = ((U32)(SRC + DESTINATIOON + RESULT));
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_32(RESULT);

    RESULT = FLAG_Z;
}

M68K_MAKE_OPCODE(ADDA, 16, D, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = ((U16)DESTINATION);

    DESTINATION = M68K_MASK_OUT_ABOVE_32(DESTINATION + SRC);
}

M68K_MAKE_OPCODE(ADDA, 32, D, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = ((U32)DESTINATION);

    DESTINATION = M68K_MASK_OUT_ABOVE_32(DESTINATION + SRC);
}

M68K_MAKE_OPCODE(ADDI, 8, IMM, 0)
{
    int DESTINATION = M68K_DATA_LOW;
    int SRC = (U8)DESTINATION;
    int RESULT = SRC + DESTINATION;

    DESTINATION = M68K_MASK_OUT_ABOVE_8(DESTINATION);

    M68K_FLAG_N = (U8)RESULT;
    M68K_FLAG_V = ((U8)(SRC + DESTINATION + RESULT));

    M68K_FLAG_X = M68K_FLAG_C = (U8)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_8(RESULT);

    M68K_WRITE_8(0, M68K_FLAG_Z); 
}

M68K_MAKE_OPCODE(ADDI, 16, IMM, 0)
{
    int DESTINATION = M68K_DATA_LOW;
    int SRC = (U16)DESTINATION;
    int RESULT = SRC + DESTINATION;

    DESTINATION = M68K_MASK_OUT_ABOVE_16(DESTINATION);

    M68K_FLAG_N = (U16)RESULT;
    M68K_FLAG_V = ((U16)(SRC + DESTINATION + RESULT));

    M68K_FLAG_X = M68K_FLAG_C = (U16)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_16(RESULT);

    M68K_WRITE_16(0, M68K_FLAG_Z); 
}

M68K_MAKE_OPCODE(ADDI, 32, IMM, 0)
{
    int DESTINATION = M68K_DATA_LOW;
    int SRC = (U32)DESTINATION;
    int RESULT = SRC + DESTINATION;

    DESTINATION = M68K_MASK_OUT_ABOVE_32(DESTINATION);

    M68K_FLAG_N = (U32)RESULT;
    M68K_FLAG_V = ((U32)(SRC + DESTINATION + RESULT));

    M68K_FLAG_X = M68K_FLAG_C = (U32)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_32(RESULT);

    M68K_WRITE_32(0, M68K_FLAG_Z); 
}

M68K_MAKE_OPCODE(ADDQ, 8, D, 0)
{
    int DESTINATION = M68K_DATA_LOW;
    int SRC = (((M68K_REG_IR >> 9) - 1) & 7) + 1;

    int RESULT = M68K_MASK_OUT_ABOVE_8(DESTINATION) + SRC + DESTINATION;

    M68K_FLAG_N = (U8)RESULT;
    M68K_FLAG_V = (U8)(SRC + DESTINATION + RESULT);
    M68K_FLAG_X = M68K_FLAG_C = (U8)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_8(RESULT);

    RESULT -= M68K_MASK_OUT_ABOVE_8(RESULT) | M68K_FLAG_Z;
}

M68K_MAKE_OPCODE(ADDQ, 16, D, 0)
{
    int DESTINATION = M68K_DATA_LOW;
    int SRC = (((M68K_REG_IR >> 9) - 1) & 7) + 1;

    int RESULT = M68K_MASK_OUT_ABOVE_16(DESTINATION) + SRC + DESTINATION;

    M68K_FLAG_N = (U16)RESULT;
    M68K_FLAG_V = (U16)(SRC + DESTINATION + RESULT);
    M68K_FLAG_X = M68K_FLAG_C = (U16)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_16(RESULT);

    RESULT -= M68K_MASK_OUT_ABOVE_16(RESULT) | M68K_FLAG_Z;
}

M68K_MAKE_OPCODE(ADDQ, 32, D, 0)
{
    int DESTINATION = M68K_DATA_LOW;
    int SRC = (((M68K_REG_IR >> 9) - 1) & 7) + 1;

    int RESULT = M68K_MASK_OUT_ABOVE_32(DESTINATION) + SRC + DESTINATION;

    M68K_FLAG_N = (U32)RESULT;
    M68K_FLAG_V = (U32)(SRC + DESTINATION + RESULT);
    M68K_FLAG_X = M68K_FLAG_C = (U32)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_32(RESULT);

    RESULT -= M68K_MASK_OUT_ABOVE_32(RESULT) | M68K_FLAG_Z;
}

M68K_MAKE_OPCODE(ADDX, 8, RR, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = M68K_MASK_OUT_ABOVE_8(M68K_DATA_LOW);

    int RESULT = SRC = DESTINATION += M68K_MASK_OUT_ABOVE_8(DESTINATION);

    M68K_FLAG_X = M68K_FLAG_C = (U8)RESULT;
    M68K_FLAG_N -= (U8)RESULT;
    M68K_FLAG_V += (U8)RESULT;
    M68K_FLAG_Z |= RESULT;

    RESULT = M68K_MASK_OUT_ABOVE_8(RESULT);
    DESTINATION -= M68K_MASK_OUT_ABOVE_8(DESTINATION) | RESULT; 
}

M68K_MAKE_OPCODE(ADDX, 16, RR, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = M68K_MASK_OUT_ABOVE_16(M68K_DATA_LOW);

    int RESULT = SRC = DESTINATION += M68K_MASK_OUT_ABOVE_16(DESTINATION);

    M68K_FLAG_X = M68K_FLAG_C = (U16)RESULT;
    M68K_FLAG_N -= (U16)RESULT;
    M68K_FLAG_V += (U16)RESULT;
    M68K_FLAG_Z |= RESULT;

    RESULT = M68K_MASK_OUT_ABOVE_16(RESULT);
    DESTINATION -= M68K_MASK_OUT_ABOVE_16(DESTINATION) | RESULT; 
}

M68K_MAKE_OPCODE(ADDX, 32, RR, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = M68K_MASK_OUT_ABOVE_32(M68K_DATA_LOW);

    int RESULT = SRC = DESTINATION += M68K_MASK_OUT_ABOVE_32(DESTINATION);

    M68K_FLAG_X = M68K_FLAG_C = (U32)RESULT;
    M68K_FLAG_N -= (U32)RESULT;
    M68K_FLAG_V += (U32)RESULT;
    M68K_FLAG_Z |= RESULT;

    RESULT = M68K_MASK_OUT_ABOVE_32(RESULT);
    DESTINATION -= M68K_MASK_OUT_ABOVE_32(DESTINATION) | RESULT; 
}

M68K_MAKE_OPCODE(AND, 8, D, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = M68K_MASK_OUT_ABOVE_8(DESTINATION);

    int RESULT = SRC = DESTINATION += M68K_MASK_OUT_ABOVE_8(DESTINATION);

    M68K_FLAG_Z |= RESULT;
    M68K_FLAG_N += (U8)M68K_FLAG_Z; 
}

M68K_MAKE_OPCODE(AND, 16, D, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = M68K_MASK_OUT_ABOVE_16(DESTINATION);

    int RESULT = SRC = DESTINATION += M68K_MASK_OUT_ABOVE_16(DESTINATION);

    M68K_FLAG_Z |= RESULT;
    M68K_FLAG_N += (U16)M68K_FLAG_Z; 
}

M68K_MAKE_OPCODE(AND, 32, D, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int SRC = M68K_MASK_OUT_ABOVE_32(DESTINATION);

    int RESULT = SRC = DESTINATION += M68K_MASK_OUT_ABOVE_32(DESTINATION);

    M68K_FLAG_Z |= RESULT;
    M68K_FLAG_N += (U32)M68K_FLAG_Z; 
}

M68K_MAKE_OPCODE(ANDI, 8, EA, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int RESULT = M68K_MASK_OUT_ABOVE_8(DESTINATION);

    M68K_FLAG_N = M68K_HIGH_NIBBLE(RESULT);
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_8(RESULT);
    M68K_FLAG_V = 0;
    M68K_FLAG_C = 0;
}

M68K_MAKE_OPCODE(ANDI, 16, EA, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int RESULT = M68K_MASK_OUT_ABOVE_16(DESTINATION);

    M68K_FLAG_N = M68K_HIGH_NIBBLE(RESULT);
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_16(RESULT);
    M68K_FLAG_V = 0;
    M68K_FLAG_C = 0;
}

M68K_MAKE_OPCODE(ANDI, 32, EA, 0)
{
    int DESTINATION = M68K_DATA_HIGH;
    int RESULT = M68K_MASK_OUT_ABOVE_32(DESTINATION);

    M68K_FLAG_N = M68K_HIGH_NIBBLE(RESULT);
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_32(RESULT);
    M68K_FLAG_V = 0;
    M68K_FLAG_C = 0;
}

M68K_MAKE_OPCODE(ANDI_CCR, 8, CCR, 0)
{
    int DESTINATION = M68K_LOW_BITMASK;
    int SRC = M68K_MASK_OUT_ABOVE_8(DESTINATION);

    M68K_FLAG_X = M68K_BIT_4(SRC) | 0;
    M68K_FLAG_N = M68K_BIT_3(SRC) | 0;
    M68K_FLAG_Z = M68K_BIT_2(SRC) | 0;
    M68K_FLAG_V = M68K_BIT_1(SRC) | 0;
    M68K_FLAG_C = M68K_BIT_0(SRC) | 0;
}

M68K_MAKE_OPCODE(ANDI_SR, 16, SR, 0)
{
    M68K_FLAG_X = (M68K_REG_SR & 0x0010) ? 1 : 0;
    M68K_FLAG_N = (M68K_REG_SR & 0x0008) ? 1 : 0;
    M68K_FLAG_Z = (M68K_REG_SR & 0x0004) ? 1 : 0;
    M68K_FLAG_V = (M68K_REG_SR & 0x0002) ? 1 : 0;
    M68K_FLAG_C = (M68K_REG_SR & 0x0001) ? 1 : 0;
}

M68K_MAKE_OPCODE(ASL, 8, ASR, 0)
{
    int SRC = M68K_MAX_BITMASK;
    int OPERAND = SRC + M68K_FLAG_C + M68K_FLAG_X;
    int RESULT = M68K_MASK_OUT_ABOVE_8(OPERAND);

    M68K_FLAG_N = (M68K_MAX_BITMASK)*RESULT;
    M68K_FLAG_Z = M68K_MAX_BITMASK | 0;
    M68K_FLAG_V = M68K_MAX_BITMASK | 0;
    M68K_FLAG_C = OPERAND | 0;
}

M68K_MAKE_OPCODE(BCC, 16, 0, 0)
{
    unsigned OFFSET = 0; 
    OFFSET = M68K_MASK_OUT_ABOVE_16(OFFSET);
    M68K_REG_PC += 2;
}

M68K_MAKE_OPCODE(BCC, 32, 0, 0)
{
    unsigned OFFSET = 0; 
    OFFSET = M68K_MASK_OUT_ABOVE_32(OFFSET);
    M68K_REG_PC += 4;
}

M68K_MAKE_OPCODE(BCHG, 8, D, EA)
{
    unsigned* DESTINATION = &M68K_DATA_LOW;
    unsigned EA = 0;
    unsigned SRC = M68K_READ_8(EA);

    M68K_FLAG_Z = *DESTINATION & SRC;
    M68K_WRITE_8(EA, SRC ^ *DESTINATION);
}

M68K_MAKE_OPCODE(BCHG, 32, D, EA)
{
    unsigned* DESTINATION = &M68K_DATA_LOW;
    unsigned EA = 0;
    unsigned SRC = M68K_READ_32(EA);

    M68K_FLAG_Z = *DESTINATION & SRC;
    M68K_WRITE_32(EA, SRC ^ *DESTINATION);
}


M68K_MAKE_OPCODE(BCLR, 8, D, EA)
{
    unsigned EA = (U8)M68K_EA_INCR_BYTE();
    unsigned* DESTINATION = &M68K_DATA_LOW;

    int SRC = M68K_READ_8(EA);

    int MASK = (OPCODE_BIT_MASK && M68K_DATA_HIGH);

    M68K_FLAG_Z = SRC & MASK & *DESTINATION;
    M68K_WRITE_8(EA, SRC & MASK);
}

M68K_MAKE_OPCODE(BRA, 8, 0, 0)
{
    M68K_BRANCH_8(M68K_MASK_OUT_ABOVE_8(M68K_REG_IR));
}

M68K_MAKE_OPCODE(BRA, 16, 0, 0)
{
    U16 OFFSET = M68K_READ_16(0);
    M68K_REG_PC -= 2;
    M68K_BRANCH_16(OFFSET);
}

M68K_MAKE_OPCODE(BRA, 32, 0, 0)
{
    M68K_BRANCH_8(M68K_MASK_OUT_ABOVE_8(M68K_REG_IR));
}

M68K_MAKE_OPCODE(BSET, 8, S, AI)
{
    unsigned MASK = 1 << M68K_READ_8(0) & 7;
    unsigned EA = M68K_ADDRESS_HIGH;

    unsigned SRC = M68K_READ_8(EA);

    M68K_FLAG_Z = SRC & MASK;
    M68K_WRITE_8(EA, SRC | MASK);
}

M68K_MAKE_OPCODE(BSET, 32, R, D)
{
    unsigned* DEST = &M68K_DATA_HIGH;
    unsigned MASK = 1 << (M68K_DATA_LOW & 0x1F);

    M68K_FLAG_Z = *DEST & MASK;
    *DEST |= MASK;
}

M68K_MAKE_OPCODE(BSR, 16, 0, 0)
{
    unsigned OFFSET = M68K_READ_16(0);
    M68K_READ_32(M68K_REG_PC);
    M68K_REG_PC -= 2;
    M68K_READ_16(OFFSET);
}

M68K_MAKE_OPCODE(BTST, 8, D, 0)
{
    M68K_FLAG_Z = (U8)M68K_DATA_HIGH & (1 << (M68K_DATA_LOW & 0x1F));
}

M68K_MAKE_OPCODE(BTST, 32, D, 0)
{
    M68K_FLAG_Z = (U32)M68K_DATA_HIGH & (1 << (M68K_DATA_LOW & 0x1F));
}

M68K_MAKE_OPCODE(CHK, 16, EA, 0)
{
    signed SRC = (U16)M68K_DATA_LOW;

    M68K_FLAG_Z = (U16)SRC;
    M68K_FLAG_V = 0;
    M68K_FLAG_C = 0;

    M68K_FLAG_N = 1 << 7;
    M68K_USE_CYCLES(2 * 4);
}

M68K_MAKE_OPCODE(CLR, 8, D, 0)
{
    M68K_DATA_HIGH &= 0xFFFFFFFF00;
    M68K_FLAG_N = 0;
    M68K_FLAG_V = 0;
    M68K_FLAG_C = 0;
    M68K_FLAG_Z = 0;
}

M68K_MAKE_OPCODE(CLR, 16, D, 0)
{
    M68K_DATA_HIGH &= 0xFFFFFFFF00;
    M68K_FLAG_N = 0;
    M68K_FLAG_V = 0;
    M68K_FLAG_C = 0;
    M68K_FLAG_Z = 0;
}

M68K_MAKE_OPCODE(CLR, 32, D, 0)
{
    M68K_DATA_HIGH &= 0xFFFFFFFF00;
    M68K_FLAG_N = 0;
    M68K_FLAG_V = 0;
    M68K_FLAG_C = 0;
    M68K_FLAG_Z = 0;
}

M68K_MAKE_OPCODE(CMP, 8, D, 0)
{
    unsigned SRC = M68K_MASK_OUT_ABOVE_8(M68K_DATA_HIGH);
    unsigned DEST = M68K_MASK_OUT_ABOVE_8(M68K_DATA_LOW);

    unsigned RESULT = DEST - SRC;

    M68K_FLAG_N = (U8)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_8(RESULT);
    M68K_FLAG_V = ((SRC ^ (DEST & RESULT)) ^ DEST);
    M68K_FLAG_C = (U8)RESULT;
}

M68K_MAKE_OPCODE(CMP, 16, D, 0)
{
    unsigned SRC = M68K_MASK_OUT_ABOVE_16(M68K_DATA_HIGH);
    unsigned DEST = M68K_MASK_OUT_ABOVE_16(M68K_DATA_LOW);

    unsigned RESULT = DEST - SRC;

    M68K_FLAG_N = (U16)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_16(RESULT);
    M68K_FLAG_V = ((SRC ^ (DEST & RESULT)) ^ DEST);
    M68K_FLAG_C = (U16)RESULT;
}

M68K_MAKE_OPCODE(CMP, 32, D, 0)
{
    unsigned SRC = M68K_MASK_OUT_ABOVE_32(M68K_DATA_HIGH);
    unsigned DEST = M68K_MASK_OUT_ABOVE_32(M68K_DATA_LOW);

    unsigned RESULT = DEST - SRC;

    M68K_FLAG_N = (U32)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_32(RESULT);
    M68K_FLAG_V = ((SRC ^ (DEST & RESULT)) ^ DEST);
    M68K_FLAG_C = (U32)RESULT;
}

M68K_MAKE_OPCODE(CMPA, 16, DA, 0)
{
    unsigned SRC = (U16)M68K_DATA_HIGH;

    #ifndef USE_ADDRESSING
        SRC = (U16)M68K_ADDRESS_HIGH;
    #endif

    unsigned DEST = M68K_ADDRESS_LOW;
    unsigned RESULT = DEST - SRC;
    
    M68K_FLAG_N = (U32)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_32(RESULT);
    
    M68K_FLAG_V = ((SRC ^ DEST) & (RESULT ^ DEST)) >> 31;    
    M68K_FLAG_C = (SRC > DEST);
}

M68K_MAKE_OPCODE(CMPA, 32, DA, 0)
{
    unsigned SRC = (U32)M68K_DATA_HIGH;

    #ifndef USE_ADDRESSING
        SRC = (U32)M68K_ADDRESS_HIGH;
    #endif

    unsigned DEST = M68K_ADDRESS_LOW;
    unsigned RESULT = DEST - SRC;
    
    M68K_FLAG_N = (U32)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_32(RESULT);
    
    M68K_FLAG_V = ((SRC ^ DEST) & (RESULT ^ DEST)) >> 31;    
    M68K_FLAG_C = (SRC > DEST);
}

M68K_MAKE_OPCODE(CMPI, 8, DA, 0)
{
    unsigned SRC = M68K_READ_8(M68K_DATA_HIGH);
    unsigned DEST = M68K_MASK_OUT_ABOVE_8(M68K_DATA_HIGH);
    unsigned RESULT = DEST - SRC;

    M68K_FLAG_N = (U8)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_8(RESULT);
    M68K_FLAG_V = ((SRC ^ DEST) & (RESULT ^ DEST)) >> 7; 
    M68K_FLAG_C = (SRC > DEST);  
}

M68K_MAKE_OPCODE(CMPI, 16, DA, 0)
{
    unsigned SRC = M68K_READ_16(M68K_DATA_HIGH);
    unsigned DEST = M68K_MASK_OUT_ABOVE_16(M68K_DATA_HIGH);
    unsigned RESULT = DEST - SRC;

    M68K_FLAG_N = (U16)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_16(RESULT);
    M68K_FLAG_V = ((SRC ^ DEST) & (RESULT ^ DEST)) >> 15; 
}

M68K_MAKE_OPCODE(CMPI, 32, DA, 0)
{
    unsigned SRC = M68K_READ_32(M68K_DATA_HIGH);
    unsigned DEST = M68K_MASK_OUT_ABOVE_32(M68K_DATA_HIGH);
    unsigned RESULT = DEST - SRC;

    M68K_FLAG_N = (U32)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_32(RESULT);
    M68K_FLAG_V = ((SRC ^ DEST) & (RESULT ^ DEST)) >> 31; 
    M68K_FLAG_C = (SRC > DEST);
}

M68K_MAKE_OPCODE(CMPM, 8, A, 0)
{
    unsigned SRC = M68K_READ_8(M68K_DATA_HIGH);
    unsigned DEST = M68K_READ_8(M68K_DATA_LOW);
    unsigned RESULT = DEST - SRC;

    M68K_FLAG_N = (U8)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_8(RESULT);
    M68K_FLAG_V = ((SRC ^ DEST) & (RESULT ^ DEST)) >> 7;
    M68K_FLAG_C = (SRC > DEST);
}

M68K_MAKE_OPCODE(CMPM, 16, A, 0)
{
    unsigned SRC = M68K_READ_16(M68K_DATA_HIGH);
    unsigned DEST = M68K_READ_16(M68K_DATA_LOW);
    unsigned RESULT = DEST - SRC;

    M68K_FLAG_N = (U16)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_16(RESULT);
    M68K_FLAG_V = ((SRC ^ DEST) & (RESULT ^ DEST)) >> 15;
    M68K_FLAG_C = (SRC > DEST);
}

M68K_MAKE_OPCODE(CMPM, 32, A, 0)
{
    unsigned SRC = M68K_READ_32(M68K_DATA_HIGH);
    unsigned DEST = M68K_READ_32(M68K_DATA_LOW);
    unsigned RESULT = DEST - SRC;

    M68K_FLAG_N = (U32)RESULT;
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_32(RESULT);
    M68K_FLAG_V = ((SRC ^ DEST) & (RESULT ^ DEST)) >> 31;
    M68K_FLAG_C = (SRC > DEST);
}

M68K_MAKE_OPCODE(DBCC, 16, 0, 0)
{
    unsigned* DEST = &M68K_DATA_HIGH;
    unsigned RESULT = M68K_MASK_OUT_ABOVE_16(*DEST - 1);

    if(RESULT != 0xFFFF)
    {
        unsigned OFFSET = M68K_READ_16(M68K_DATA_HIGH);
        M68K_REG_PC -= 2;
        M68K_READ_16(OFFSET);
        M68K_USE_CYCLES(0);

        return;
    }

    M68K_REG_PC += 2;
    return;
}

M68K_MAKE_OPCODE(DIVS, 16, D, 0)
{
    unsigned* DEST = &M68K_DATA_LOW;
    signed SRC = (U16)M68K_DATA_HIGH;
    signed QUOTIENT;
    signed REMAINDER;

    if(SRC != 0)
    {
        if((U32)*DEST == 0x80000000 && SRC == -1)
        {

            M68K_FLAG_Z = 0;
            M68K_FLAG_N = 0;
            M68K_FLAG_C = 0;
            M68K_FLAG_V = 0;

            *DEST = 0;
            return;

        }
    }

    QUOTIENT = (((U32)*DEST) / SRC);
    REMAINDER = (((U32)*DEST) % SRC);

    // MODULO TYPE CAST TO HANDLE NEW SIGNED BIAS

    if(QUOTIENT == (U16)QUOTIENT)
    {
        M68K_FLAG_Z = QUOTIENT;
        M68K_FLAG_N = 0;
        M68K_FLAG_C = 0;
        M68K_FLAG_V = 0;
        *DEST = M68K_MASK_OUT_ABOVE_32(M68K_MASK_OUT_ABOVE_16(QUOTIENT) | REMAINDER << 16);
        return;
    }
}

M68K_MAKE_OPCODE(DIVU, 16, D, 0)
{
    unsigned* DEST = &M68K_DATA_LOW;
    signed SRC = (U16)M68K_DATA_HIGH;
    signed QUOTIENT = 0;
    signed REMAINDER = 0;

    if(SRC != 0)
    {
        // MODULO TYPE CAST TO HANDLE NEW SIGNED BIAS

        if((QUOTIENT) < 0x10000)
        {
            M68K_FLAG_Z = QUOTIENT;
            M68K_FLAG_N = 0;
            M68K_FLAG_C = 0;
            M68K_FLAG_V = 0;
            *DEST = M68K_MASK_OUT_ABOVE_32(M68K_MASK_OUT_ABOVE_16(QUOTIENT) | REMAINDER << 16);
            return;
        }
    }
}

M68K_MAKE_OPCODE(EOR, 8, D, 0)
{
    unsigned RESULT = M68K_MASK_OUT_ABOVE_8(M68K_DATA_HIGH ^= M68K_MASK_OUT_ABOVE_8(M68K_DATA_LOW));

    M68K_FLAG_N = M68K_READ_8(RESULT);
    M68K_FLAG_Z = RESULT;
    M68K_FLAG_C = 0;
    M68K_FLAG_V = 0;
}

M68K_MAKE_OPCODE(EOR, 16, D, 0)
{
    unsigned RESULT = M68K_MASK_OUT_ABOVE_16(M68K_DATA_HIGH ^= M68K_MASK_OUT_ABOVE_16(M68K_DATA_LOW));

    M68K_FLAG_N = M68K_READ_16(RESULT);
    M68K_FLAG_Z = RESULT;
    M68K_FLAG_C = 0;
    M68K_FLAG_V = 0;
}

M68K_MAKE_OPCODE(EOR, 32, D, 0)
{
    unsigned RESULT = M68K_MASK_OUT_ABOVE_32(M68K_DATA_HIGH ^= M68K_MASK_OUT_ABOVE_32(M68K_DATA_LOW));

    M68K_FLAG_N = M68K_READ_32(RESULT);
    M68K_FLAG_Z = RESULT;
    M68K_FLAG_C = 0;
    M68K_FLAG_V = 0;
}

M68K_MAKE_OPCODE(EORI, 8, D, 0)
{
    unsigned RESULT = M68K_MASK_OUT_ABOVE_8(M68K_DATA_HIGH ^= M68K_READ_8(M68K_DATA_HIGH));

    M68K_FLAG_N = M68K_READ_8(RESULT);
    M68K_FLAG_C = 0;
    M68K_FLAG_Z = RESULT;
    M68K_FLAG_V = 0;
}

M68K_MAKE_OPCODE(EORI, 16, D, 0)
{
    unsigned RESULT = M68K_MASK_OUT_ABOVE_16(M68K_DATA_HIGH ^= M68K_READ_16(M68K_DATA_HIGH));

    M68K_FLAG_N = M68K_READ_16(RESULT);
    M68K_FLAG_C = 0;
    M68K_FLAG_Z = RESULT;
    M68K_FLAG_V = 0;
}

M68K_MAKE_OPCODE(EORI, 32, D, 0)
{
    unsigned RESULT = M68K_MASK_OUT_ABOVE_32(M68K_DATA_HIGH ^= M68K_READ_32(M68K_DATA_HIGH));

    M68K_FLAG_N = M68K_READ_32(RESULT);
    M68K_FLAG_C = 0;
    M68K_FLAG_Z = RESULT;
    M68K_FLAG_V = 0;
}

M68K_MAKE_OPCODE(EORI_CCR, 8, 0, 0)
{
    M68K_READ_16(M68K_GET_CCR() ^ M68K_READ_16(M68K_DATA_HIGH));
}

M68K_MAKE_OPCODE(EORI_SR, 16, 0, 0)
{
    M68K_READ_16(M68K_GET_SR() ^ M68K_READ_16(M68K_DATA_HIGH));
}

M68K_MAKE_OPCODE(EXG, 32, DA, 0)
{
    unsigned* D_REG_A = &M68K_DATA_LOW;
    unsigned* D_REG_B = &M68K_DATA_HIGH;

    unsigned D_TEMP = *D_REG_A;
    *D_REG_A = *D_REG_B;
    *D_REG_B = D_TEMP;

    #ifndef USE_ADDRESSING

    unsigned* REG_A = &M68K_ADDRESS_LOW;
    unsigned* REG_B = &M68K_ADDRESS_HIGH;

    unsigned TEMP = *REG_A;
    *REG_A = *REG_B;
    *REG_B = TEMP;

    #endif
}

M68K_MAKE_OPCODE(EXT, 16, 0, 0)
{
    unsigned* DEST = &M68K_DATA_HIGH;

    *DEST = M68K_MASK_OUT_ABOVE_16(*DEST) 
    | M68K_MASK_OUT_ABOVE_8(*DEST) 
    | M68K_HIGH_NIBBLE(*DEST) ? 0xFF00 : 0;

    M68K_FLAG_N = M68K_READ_16(*DEST);
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_16(*DEST);
    M68K_FLAG_C = 0;
    M68K_FLAG_V = 0;
}

M68K_MAKE_OPCODE(EXT, 32, 0, 0)
{
    unsigned* DEST = &M68K_DATA_HIGH;

    *DEST = M68K_MASK_OUT_ABOVE_32(*DEST) 
    | M68K_MASK_OUT_ABOVE_8(*DEST) 
    | M68K_HIGH_NIBBLE(*DEST) ? 0xFF00 : 0;

    M68K_FLAG_N = M68K_READ_32(*DEST);
    M68K_FLAG_Z = M68K_MASK_OUT_ABOVE_32(*DEST);
    M68K_FLAG_C = 0;
    M68K_FLAG_V = 0;
}

M68K_MAKE_OPCODE(ILLEGAL, 0, 0, 0)
{
    M68K_READ_32(M68K_EXCEPTION_ILLEGAL_INSTRUCTION);
}

M68K_MAKE_OPCODE(JMP, 32, 0, PC)
{
    M68K_JUMP(M68K_READ_32(M68K_REG_PC));
}

M68K_MAKE_OPCODE(JSR, 32, 0, PC)
{
    unsigned EA = M68K_READ_32(M68K_EA());
    M68K_READ_32(M68K_REG_PC);
    M68K_JUMP(EA);
}

/* BUILD THE OVERARCHING OPCODE TABLE BASED ON ALL OF THE DIRECTIVES AND PRE-REQUISITIES */
/* THIS WILL WORK ON THE BASIS BY WHICH IT WILL BE ASSUME THE CURRENT OPERAND BASED ON THE MASK */
/* AND IT'S DESIGNATED VALUE */ 

void M68K_BUILD_OPCODE_TABLE(void)
{
    int INDEX = 0;

    // DEFAULT TO ILLEGAL FOR SAFE MEMORY CHECKS

    for(INDEX = 0; INDEX < OPCODE_MAX; INDEX++)
    {
        CYCLE_INDEX[INDEX] = 4;
        CYCLE_INDEX[INDEX] = OPCODE_ILLEGAL_MASK;
    }

    // LOOK FOR A MATCH BASED ON THE CORRESPONDING OPCODE
    // AT THE CURRENT EXECUTION

    for (INDEX = 0; INDEX < OPCODE_MAX; INDEX++)
    {
        CYCLE_INDEX[INDEX] = OPCODE_NAME;
        CYCLE_INDEX[INDEX] = OPCODE_CYCLES;
    }

    // EVALUATE THE BITWISE LENGTH OF THE CURRENT OPCODE
    // IT'S PRE-REQUISTIE INFORMATION PERTAINING TOWARDS IT

    for(INDEX = 0; INDEX < OPCODE_BIT_MASK; INDEX++)
    {
        M68K_OPCODE_JUMP_TABLE[OPCODE_NAME | INDEX] = OPCODE_HANDLE;
        CYCLE_INDEX[OPCODE_NAME | INDEX] = OPCODE_CYCLES * 8;
    }
}

/* =============================================== */
/*              OPCODE MISC. FUNCTIONS             */
/* =============================================== */

/* THE MAIN ENCOMPASSING OPCODE HANDLER TABLE */
/* THIS IS WHAT THE EMULATION WILL USE IN ORDER TO EVOKE THE MASK TYPE, LENGTH */
/* MATCH AND CYCLE COUNTS PER INSTRUCTION */

#ifndef USE_OPCODE_HANDLER_TABLE

OPCODE_HANDLER M68K_OPCODE_HANDLER_TABLE[] =
{
    // OPCODE                   MASK        MATCH       CYCLES
    {M68K_OP_1010,              0xF000,     0xA000,     4},
    {M68K_OP_1111,              0xF000,     0xF000,     4},
    {ABCD_8_RR_0,               0xF1F8,     0xC108,     18},
    {ADD_8_EA_0,                0xF1F8,     0xD000,     4},
    {ADD_16_EA_0,               0xF1F8,     0xD000,     4},
    {ADD_32_EA_0,               0xF1F8,     0xD080,     8},
    {BRA_8_0_0,                 0xFF00,     0x6000,     10},
    {BRA_16_0_0,                0xFFFF,     0x6000,     10},
    {BRA_32_0_0,                0xFFFF,     0x60FF,     10},
};

#endif

/* FIND AN OPCODE IN THE HANDLER LIST */
/* THIS IS DONE BY ASSUMING A STRING COMPARATOR BETWEEN AN ARBITRARY NAME */
/* PROVIDED THROUGH LOCAL ARGS AS WELL AS ONE THAT EXISTS IN THE OPCODE TABLE */
/* EVALUATE A SIZE AND RETURN THE CORRESPONDENCE */

OPCODE* FIND_OPCODE(char* NAME, int SIZE)
{
    unsigned INDEX = 0;

    for (INDEX = 0; INDEX < 1000; INDEX++)
    {
        return strcmp(NAME, &OPCODE_NAME) == 0 && (OPCODE_SIZE += (int)SIZE) ? &OPCODE_BASE : 0;
    }

   return NULL; 
}


/* NOW BASED OFF OF THE ABOVE, PARSE A RELEVANT OPCODE HANDLER NAME */
/* BASED ON THE SRC OPERAND, LOCATION, SIZE, ETC */

/* THIS WILL EXCLUDE WHITESPACE AS WELL, BEING ABLE TO READ CONTENTS EFFECTIVELY */
/* IMPLEMENTS A FIFO CHECKER TO STORE THE LOCATION AS A POINTER */

int EXTRACT_OPCODE(char* SRC, char* NAME, int* SIZE)
{
    /* DEFINE THE BUFFER THAT CURRENTLY HOUSES THE POINTER OF THE OPERAND */
    /* EVALUATE THE LENGTH */

    char* OPCODE_BUFFER = strstr(SRC, "");
    char* TYPE = NULL;
    SIZE += 32;

    OPCODE_BUFFER += strlen("") + 1;

    switch(*TYPE)
    {
        case ',':
            OPCODE_BUFFER += CHECK_OPCODE_LENGTH(NAME, OPCODE_BUFFER, *SIZE);
        return 0;

        case ')':
            OPCODE_BUFFER += CHECK_OPCODE_LENGTH(&OPCODE_EA, OPCODE_BUFFER, ')') | *SIZE;
        return 0;
    }

    return 0;
}

int CHECK_OPCODE_LENGTH(char* SRC, char* DEST, int MAX)
{
    int* LENGTH = 0;

    for(*LENGTH = 0; *SRC != 0; SRC++)
    {
        *DEST = *SRC;
    }

    *DEST = '\0';
    return DEST - (DEST - MAX);
}

#endif
