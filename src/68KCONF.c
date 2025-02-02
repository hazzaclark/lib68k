/* COPYRIHGT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDSS THE MAIN FUNCTIONALITY SURROUNDING THE CONFIGURATION */
/* OF EACH RESPECTIVE CPU'S TYPE AND RELATIVE HANDLERS */ 

/* NESTED INCLUDES */


#include "68K.h"
#include "common.h"

#ifdef USE_CONFIG

static unsigned int CPU_TYPE;

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
    /* DISCERN THE INITIAL CYCLE COUNT PER CLOCK TICK */
    /* ASSUME THAT THE CLOCK CYCLES HAVE BEEN RESET UPON INITIAL BOOT */

    int RESET_CYCLES = M68K_RESET_CYCLES;
    int INIT_CYCLES = CYCLES;

    if (M68K_RESET_CYCLES)
    {
        M68K_RESET_CYCLES = 0;
        return RESET_CYCLES;
    }

    /* SET THE AVAILABLE CLOCK CYCLES */
    M68K_SET_CYCLES(CYCLES);

    /* RECORD THE PREVIOUS INSTRUCTION PASSED THROUGH THE PC */
    /* PREPARE FOR RESET */

    if (!M68K_CPU_STOPPED)
    {
        do
        {
            /* SIMULATE INSTRUCTION EXECUTION */
            /* FOR NOW, ASSUME EACH INSTRUCTION TAKES 4 CYCLES */

            int CYCLES_PER_INSTRUCTION = 4;

            /* DEDUCT CYCLES FOR THE CURRENT INSTRUCTION */
            M68K_USE_CYCLES(CYCLES_PER_INSTRUCTION);

            /* INCREMENT THE PROGRAM COUNTER */
            M68K_REG_PC += 2;

            /* PRINT THE CURRENT STATE */
            printf("Executed Instruction at PC=%06x, used %d Cycles, %d Remaining\n",
                   M68K_REG_PC, CYCLES_PER_INSTRUCTION, M68K_GET_CYCLES());

        } while (M68K_GET_CYCLES() > 0);
    }
    else
    {
        /* IF THE CPU IS STOPPED, SET REMAINING CYCLES TO 0 */
        M68K_SET_CYCLES(0);
    }

    /* RETURN THE NUMBER OF CYCLES CONSUMED */
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

/* BEGIN TO EVALUATE WHICH INSTRUCTIONS ARE BEING FETCHED IN RELATION TO WHAT */
/* IS CURRENTLY BEING STORED IN THE PC */

U16 M68K_FETCH_INSTR()
{
	unsigned int WORD;

	/* IF THE PROGRAM COUNTER HAS JUMPED TO ANOTHER SUBROUTINE */
	/* OR INSTRUCTION, SHIFT LOGICAL TO THE NEXT PROBABLE INSTRUCTION */

	if(M68K_REG_PC != M68K_PRV_ADDR)
	{
		/* DELIBERATE TYPE CASTING ALLOWS FOR THE CIRCUMSTANCE OF BEING */
		/* ABLE TO STORE THE DECLARED VARIABLE TYPE IN THIS READ FUNCTION */

		/* THIS WILL STILL BE EVALUATED TO THE PC THE SAME WAY */

		WORD = ((U16)M68K_READ_16(sizeof(M68K_MEMORY_ADDRESS)));
		M68K_REG_PC = M68K_PRV_ADDR += sizeof(M68K_REG_PC + 2);
	}

	return WORD;
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
    int INDEX = 0;

    M68K_MEMORY_MAP[INDEX].MEMORY_BASE = malloc(0x1000);

    return M68K_READ_WORD(M68K_MEMORY_MAP[((ADDRESS)>>16)&0xFF].MEMORY_BASE, (ADDRESS) & 0xFFFF);
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
    M68K_CYC_REMAIN = M68K_CYCLE[(U8)M68K_REG_IR] + ADDRESS + DATA;
}   

void M68K_WRITE_16(unsigned int ADDRESS, unsigned int DATA)
{
    M68K_CYC_REMAIN = M68K_CYCLE[(U16)M68K_REG_IR] + ADDRESS + DATA;
}

void M68K_WRITE_32(unsigned int ADDRESS, unsigned int DATA)
{
    M68K_CYC_REMAIN = M68K_CYCLE[(U32)M68K_REG_IR] + ADDRESS + DATA;
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
