/* COPYRIHGT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDSS THE MAIN FUNCTIONALITY SURROUNDING THE CONFIGURATION */
/* OF EACH RESPECTIVE CPU'S TYPE AND RELATIVE HANDLERS */ 

/* NESTED INCLUDES */

#include "68K.h"
#include "68KOPCODE.h"
#include "common.h"
#include "util.h"

#ifdef USE_CONFIG
#ifdef USE_OPCODE

static U8 MEMORY[MAX_MEMORY_SIZE];
static unsigned int CPU_TYPE;

#define			M68K_CYCLE_RANGE_MIN		2
#define			M68K_CYCLE_RANGE_MAX		16

#define NUM_OPCODE_HANDLERS (sizeof(M68K_OPCODE_HANDLER_TABLE) / sizeof(M68K_OPCODE_HANDLER_TABLE[0]))

U8 M68K_VECTOR_TABLE[5][256] =
{
	{ 
		  0,                                                                  /*  0: RESET - INITIAL STACK POINTER                      */
		  4,                                                                  /*  1: RESET - INITIAL PROGRAM COUNTER                    */
		 50,                                                                  /*  2: BUS ERROR                                          */
		 50,                                                                  /*  3: ADDRESS ERROR                                      */
		 34,                                                                  /*  4: ILLEGAL INSTR                                      */
		 38,                                                                  /*  5: ZERO DIV                                           */
		 40,                                                                  /*  6: CHK                                                */
		 34,                                                                  /*  7: TRAPV                                              */
		 34,                                                                  /*  8: PRIV VIO                                           */
		 34,                                                                  /*  9: TRACE                                              */
		 34,                                                                  /* 10: 1010                                               */
		 34,                                                                  /* 11: 1111                                               */
		  4,                                                                  /* 12: RESERVED                                           */
		  4,                                                                  /* 13: CPV                                                */
		  4,                                                                  /* 14: FMT ERROR                                          */
		 44,                                                                  /* 15: UINIT                                              */
		  4,                                                                  /* 16: RESERVED                                           */
		  4,                                                                  /* 17: RESERVED                                           */
		  4,                                                                  /* 18: RESERVED                                           */
		  4,                                                                  /* 19: RESERVED                                           */
		  4,                                                                  /* 20: RESERVED                                           */
		  4,                                                                  /* 21: RESERVED                                           */
		  4,                                                                  /* 22: RESERVED                                           */
		  4,                                                                  /* 23: RESERVED                                           */
		 44,                                                                  /* 24: SPUR IRQ                                           */
		 44,                                                                  /* 25: LVL 1 INTERRUPT VECTOR                             */
		 44,                                                                  /* 26: LVL 2 INTERRUPT VECTOR                             */
		 44,                                                                  /* 27: LVL 3 INTERRUPT VECTOR                             */
		 44,                                                                  /* 28: LVL 4 INTERRUPT VECTOR                             */
		 44,                                                                  /* 29: LVL 5 INTERRUPT VECTOR                             */
		 44,                                                                  /* 30: LVL 6 INTERRUPT VECTOR                             */
		 44,                                                                  /* 31: LVL 7 INTERRUPT VECTOR                             */
		 34,                                                                  /* 32: TRAP #0                                            */
		 34,                                                                  /* 33: TRAP #1                                            */
		 34,                                                                  /* 34: TRAP #2                                            */
		 34,                                                                  /* 35: TRAP #3                                            */
		 34,                                                                  /* 36: TRAP #4                                            */
		 34,                                                                  /* 37: TRAP #5                                            */
		 34,                                                                  /* 38: TRAP #6                                            */
		 34,                                                                  /* 39: TRAP #7                                            */
		 34,                                                                  /* 40: TRAP #8                                            */
		 34,                                                                  /* 41: TRAP #9                                            */
		 34,                                                                  /* 42: TRAP #10                                           */
		 34,                                                                  /* 43: TRAP #11                                           */
		 34,                                                                  /* 44: TRAP #12                                           */
		 34,                                                                  /* 45: TRAP #13                                           */
		 34,                                                                  /* 46: TRAP #14                                           */
		 34,                                                                  /* 47: TRAP #15                                           */
		  4,                                                                  /* 48: FP BRAS                                            */
		  4,                                                                  /* 49: FP INEXACT                                         */
		  4,                                                                  /* 50: FP ZERO DIV                                        */
		  4,                                                                  /* 51: FP UNDERFLOW                                       */
		  4,                                                                  /* 52: FP OPERAND FLOW                                    */
		  4,                                                                  /* 53: FP OVERFLOW                                        */
		  4,                                                                  /* 54: FP NAN                                             */
		  4,                                                                  /* 55: FP UNIMPLEMENTED DATA TYPE                         */
		  4,                                                                  /* 56: MMU ERROR                                          */
		  4,                                                                  /* 57: MMU ILLEGAL OP ERROR                               */
		  4,                                                                  /* 58: MMU ACCESS VIO                                     */
		  4,                                                                  /* 59: RESERVED                                           */
		  4,                                                                  /* 60: RESERVED                                           */
		  4,                                                                  /* 61: RESERVED                                           */
		  4,                                                                  /* 62: RESERVED                                           */
		  4,                                                                  /* 63: RESERVED                                           */
		                                                                      /* 64-255: USER STACK SPACE                               */
                                                                              
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
	},
};

/* SET THE CPU TYPE BASED ON HTE PRE-REQUISTIES DETERMINED BY THEIR RESPECTIVE CHARACTERISTICS */
/* EACH OF THESE CHARACTERISTICS REFER TO THE INITIALISATION OF THE CPU'S EXECUTION */

/* MOREOVER, THIS IS SPECIFC TO THE RESPECTIVE CHARACTERISTICS OF THE STATUS REGISTER */
/* OF EACH CPU TYPE, WHICH HANDLES EXCEPTIONS AND HANDLES DIFFERENTLY BASED ON DIFFERENT ADDRESSABLE MODES */

/* SEE: https://www.nxp.com/docs/en/reference-manual/MC68000UM.pdf#page=17 */

void M68K_SET_CPU_TYPE(unsigned TYPE)
{
    switch (TYPE)
    {
        case M68K_CPU_000:
            CPU_TYPE = M68K_CPU_000;
            M68K_SR_MASK = 0x2700;
            M68K_ADDRESS_MASK += 0x00FFFFFF;
            M68K_CYCLE = CPU.INSTRUCTION_CYCLES;
            M68K_CYC_EXCE = CPU.CYCLE_EXCEPTION;
            M68K_RESET_LVL += 256;
            return;

        case M68K_CPU_010:
            CPU_TYPE = M68K_CPU_010;
            M68K_SR_MASK = 0x2700;
            M68K_ADDRESS_MASK += 0x00FFFFFF;
            M68K_CYCLE = CPU.INSTRUCTION_CYCLES;
            M68K_CYC_EXCE = CPU.CYCLE_EXCEPTION;
            M68K_RESET_LVL += 256;
            return;

        case M68K_CPU_020:
            CPU_TYPE = M68K_CPU_020;
            M68K_SR_MASK += 0xF71F;
            M68K_ADDRESS_MASK += 0xFFFFFFFF;
            M68K_CYCLE = CPU.INSTRUCTION_CYCLES;
            M68K_CYC_EXCE = CPU.CYCLE_EXCEPTION;
            M68K_RESET_LVL += 512;
            return;

        default:
            break;
    }
}

/* EXECUTE THE MAIN INITIALISATION INSTRUCTIONS USED ON STARTUP TO */
/* DETERMINE THE CORRESSPONDING AMOUNT OF CYCLES PER CLOCK TICK */

/* SEE RESET COROUTINE: https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf#page=537 */

int M68K_EXEC(int CYCLES) 
{
    int RESET_CYCLES = M68K_RESET_CYCLES;
    int INIT_CYCLES = CYCLES;

    if (M68K_RESET_CYCLES) 
    {
        M68K_RESET_CYCLES = 0;
        return RESET_CYCLES;
    }

    M68K_SET_CYCLES(CYCLES);

    if (!M68K_CPU_STOPPED) 
    {
        while (M68K_GET_CYCLES() > 0) 
        {
            U16 INSTRUCTION = M68K_FETCH_INSTR();
			int CYCLES_USED = M68K_EXECUTE_INSTRUCTION(INSTRUCTION);
			M68K_USE_CYCLES(CYCLES_USED);
            printf("Executing instruction: 0x%04X at PC: 0x%04X\n", INSTRUCTION, M68K_REG_PC - 2);
        }
    } 
    else 
    {
        M68K_SET_CYCLES(0);
    }

    return INIT_CYCLES - M68K_GET_CYCLES();
}

int M68K_CYCLES_RUN(void)
{
    return M68K_GET_CYCLES() -= *(int*)M68K_CYCLE;
}

int M68K_CYCLES_REMAINING(void)
{
    return M68K_GET_CYCLES();
}

int M68K_EXECUTE_INSTRUCTION(U16 INSTRUCTION)
{
    for (size_t i = 0; i < sizeof(M68K_OPCODE_HANDLER_TABLE[i]) / sizeof(OPCODE_HANDLER); i++) 
    {
        OPCODE_HANDLER* HANDLER = &M68K_OPCODE_HANDLER_TABLE[i];

        if ((INSTRUCTION & HANDLER->MASK) == HANDLER->MATCH) 
        {
            HANDLER->HANDLER();
            return HANDLER->CYCLES;
        }
    }

    printf("Unknown instruction: 0x%04X\n", INSTRUCTION);
    return 0; 
}

/* BEGIN TO EVALUATE WHICH INSTRUCTIONS ARE BEING FETCHED IN RELATION TO WHAT */
/* IS CURRENTLY BEING STORED IN THE PC */

U16 M68K_FETCH_INSTR()
{
	/* FETCH THE NEXT INSTRUCTION FROM MEMORY */
    U16 INSTRUCTION = M68K_READ_16(M68K_REG_PC);
	printf("Fetched instruction: 0x%04X at PC: 0x%04X\n", INSTRUCTION, M68K_REG_PC);

    /* INCREMENT THE PROGRAM COUNTER */
    M68K_REG_PC += 2;

    return INSTRUCTION;
}

/* THIS FUNCTION DISCERNS THE FUNCTIONALITY BASED ON PROVIDING PULSE TO THE RESET LINE */
/* ON THE CPU BASED ON EACH RESPECTIVE REGISTER */

/* SEE: RESET OPERATION - https://www.nxp.com/docs/en/reference-manual/MC68000UM.pdf#page=75 */

void M68K_PULSE_RESET(void)
{
	M68K_CPU_STOPPED = 0;
	M68K_SET_CYCLES(0);

	/* SOMEHOW THIS WORKS */
	/* BASED ON DOCUMENTATION, EMULATE CPU TYPE SPECIFC CHARACTERISTICS */

	if(CPU_TYPE == M68K_CPU_010)
	{
		M68K_REG_VBR = (S16)0x0000;
	}

	M68K_FLAG_T1 = M68K_FLAG_T0 = 0;
	M68K_FLAG_INT_LVL = 0;
	M68K_REG_VBR = 0;
	M68K_JUMP(0);

	M68K_REG_SP += (int)M68K_READ_32(0);
	M68K_REG_PC += (int)M68K_READ_32(0);

	M68K_RESET_CYCLES += M68K_CYC_EXCE[0];
}

/* SIMILAR TO PULSE RESET EXCEPT FOR THE HALT LINE */

void M68K_PULSE_HALT(void)
{
	M68K_CPU_STOPPED |= 2;
}

/*===============================================================================*/
/*							68000 HELPER FUNCTIONS							     */
/*===============================================================================*/
/*      THE FOLLOWING FUNCTIONS PERTAIN TOWARDS THE BITWISE WAYS BY WHICH        */
/* INSTRUCTIONS WILL BE HANDLED - USING THE MEMORY MAP WHICH WILL DYNAMICALLY    */
/*          ALLOCATE THE CORRESPONDING MEMORY FOR THE INSTRUCTION                */
/*===============================================================================*/

unsigned int M68K_READ_8(unsigned int ADDRESS)
{
    int INDEX = 0;

    M68K_MEMORY_MAP[INDEX].MEMORY_BASE = malloc(0x1000);
    return M68K_READ_BYTE(M68K_MEMORY_MAP[((ADDRESS)>>16)&0xFF].MEMORY_BASE, (ADDRESS) & 0xFFFF);
}

unsigned int M68K_READ_16(unsigned int ADDRESS)
{
    U16 const HI = M68K_READ_8(ADDRESS + 1);
	U16 const LO = M68K_READ_8(ADDRESS + 0);

	return (HI << 8) | LO;
}

unsigned int M68K_READ_32(unsigned int ADDRESS)
{
    int INDEX = 0;

    M68K_MEMORY_MAP[INDEX].MEMORY_BASE = malloc(0x1000);
    return M68K_READ_WORD_LONG(M68K_MEMORY_MAP[((ADDRESS)>>16)&0xFF].MEMORY_BASE, (ADDRESS) & 0xFFFF);
}

/* THE WRITE FUNCTIONS WILL LOOK TO DISCERN HOW MANY CYCLE TICKS ARE LEFT RELATIVE TO THE INSTRUCTION */
/* STORE THE RESULT TO EACH RELATIVE BTIWISE VALUE IN THE INDEX REGISTER */

void M68K_WRITE_8(unsigned int ADDRESS, unsigned int DATA) 
{
    MEMORY[ADDRESS] = (U8)DATA;
    printf("Wrote Byte: 0x%02X to address 0x%04X\n", DATA, ADDRESS);
}

void M68K_WRITE_16(unsigned int ADDRESS, unsigned int DATA)
{
    MEMORY[ADDRESS] = (U16)DATA;
    printf("Wrote Byte: 0x%02X to address 0x%04X\n", DATA, ADDRESS);
}

void M68K_WRITE_32(unsigned int ADDRESS, unsigned int DATA)
{
    MEMORY[ADDRESS] = (U32)DATA;
    printf("Wrote Byte: 0x%02X to address 0x%04X\n", DATA, ADDRESS);
}

void M68K_BRANCH_8(unsigned OFFSET)
{
	M68K_REG_PC += (U8)OFFSET;
}

void M68K_BRANCH_16(unsigned OFFSET)
{
	M68K_REG_PC += (U16)OFFSET;
}

void M68K_BRANCH_32(unsigned OFFSET)
{
	M68K_REG_PC += OFFSET;
}

#endif
#endif
