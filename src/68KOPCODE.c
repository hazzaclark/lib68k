/* COPYRIHGT (C) HARRY CLARK 2024 */

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


M68K_MAKE_OPCODE(ADD, 8, ER, 0)
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

M68K_MAKE_OPCODE(ADD, 16, ER, 0)
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

M68K_MAKE_OPCODE(ADD, 32, ER, 0)
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

M68K_MAKE_OPCODE(ADDI, 8, D, 0)
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

M68K_MAKE_OPCODE(ADDI, 16, D, 0)
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

M68K_MAKE_OPCODE(ADDI, 32, D, 0)
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
