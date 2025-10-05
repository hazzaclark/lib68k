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

unsigned CPU_TYPE = M68K_CPU_TYPE;

// 04/10/25 - WHILE I DID MENTION THAT THIS WAS STRICTLY
// USED FOR THE MEGA DRIVE, WE CAN ACTUALLY LEVERAGE THIS 
// ADJACENT FROM ANY SORT OF SYSTEMS EMULATION
//
// THE 68000 WILL ALWAYS LEVERAGE THE FIRST 256 BYTES
// OF A PROGRAM TO DELEGATE THE ESSENTIALS SUCH AS SP AND PC
//
// SUCH IS THE CASE WITH MY RESET FUNCTIONALITY, WE CAN PROMPTLY
// USE THIS TABLE TO DELEGATE THE APPROPRIATE AMOUNT OF CYCLES NEEDED
// TO FLUSH THE COROURTINE PROPERLY - RATHER THAN ALWAYS PRESUPPOSING A HARD RESET
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
            M68K_ADDRESS_MASK = 0x00FFFFFF;
            break;

        case M68K_CPU_010:
            CPU_TYPE = M68K_CPU_010;
            M68K_SR_MASK = 0x2700;
            M68K_ADDRESS_MASK = 0x00FFFFFF;
            break;

        default:
			fprintf(stderr, "UNRECOGNISED TYPE %d\n", CPU_TYPE);
            exit(1);
    }

	printf("\nCPU INFORMATION:	\n");
	printf("  CPU SET TO TYPE:  %u -> %s\n", CPU_TYPE, 
       CPU_TYPE == M68K_CPU_000 ? "68000" : 
       CPU_TYPE == M68K_CPU_010 ? "68010" : "UNKNOWN");

	printf("  SUPERVISOR MODE:  %s\n\n", M68K_USE_SUPERVISOR ? "ENABLED" : "DISABLED");
}

/* THIS FUNCTION DISCERNS THE FUNCTIONALITY BASED ON PROVIDING PULSE TO THE RESET LINE */
/* ON THE CPU BASED ON EACH RESPECTIVE REGISTER */

/* SEE: RESET OPERATION - https://www.nxp.com/docs/en/reference-manual/MC68000UM.pdf#page=75 */
/* https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf#page=537 */

void M68K_PULSE_RESET(void)
{
	M68K_CPU_STOPPED = 0;
	M68K_FLAG_INT_LVL = 0;

	M68K_REG_SP = READ_IMM_32();
	M68K_REG_PC = READ_IMM_32();
	M68K_JUMP(M68K_REG_PC);

	#if M68K_USE_SUPERVISOR
	M68K_SET_S_FLAG(M68K_FLAG_S);
	#endif

	M68K_SET_CPU_TYPE(CPU_TYPE);
	M68K_BASE_RES_HOOK(M68K_FLAG_T0, M68K_FLAG_T1, M68K_REG_PC, M68K_REG_SP);
	M68K_USE_CYCLES(M68K_EXEC_VECTOR_TABLE[0][0]);
}

/* SIMILAR TO PULSE RESET EXCEPT FOR THE HALT LINE */

void M68K_PULSE_HALT(void)
{
	M68K_CPU_STOPPED |= 2;
}

void M68K_CLEAR_HALT(void)
{
	M68K_CPU_STOPPED &= ~2;
}

/*==============================================================================*/
/*                           68000 HELPER FUNCTIONS                             */
/*==============================================================================*/
/*      THE FOLLOWING FUNCTIONS PERTAIN TOWARDS THE BITWISE WAYS BY WHICH       */
/* INSTRUCTIONS WILL BE HANDLED - USING THE MEMORY MAP WHICH WILL DYNAMICALLY   */
/*          ALLOCATE THE CORRESPONDING MEMORY FOR THE INSTRUCTION               */
/*==============================================================================*/

unsigned int READ_IMM_8(void)
{
	return M68K_MASK_OUT_ABOVE_8(READ_IMM_16());
}

unsigned int READ_IMM_16(void) 
{
	int VALUE = M68K_READ_16(M68K_REG_PC);
	M68K_REG_PC += 2;
	return VALUE; 
}

unsigned int READ_IMM_32(void) 
{
	int VALUE = M68K_READ_32(M68K_REG_PC);
	M68K_REG_PC += 4;
	return VALUE;  
}

void M68K_PUSH_SP(unsigned VALUE)
{
	M68K_REG_SP = READ_IMM_16();
	M68K_WRITE_32(M68K_REG_SP, VALUE);
}

// DIFFERENCE BETWEEN THE FOLLOWING TWO FUNCTIONS
// IS THAT THE STATUS REGISTER WILL ALWAYS ENCOMPASS
// A 16 BIT SIGNED VALUE
//
// DIFFERENTIATE BETWEEN THE SIGNEDNESS BY ACCOMMODATING
// FOR THE NEXT POWER TO VALIDATE THEIR OFFSETS (JUST LIKE WITH IMM READS)

unsigned int M68K_PULL_SR(void)
{
	unsigned SR = M68K_REG_SP;
	M68K_REG_SP = M68K_MASK_OUT_BELOW_16(M68K_REG_SP + 2);
	return M68K_READ_16(SR);
}

unsigned int M68K_PULL_SP(void)
{
	unsigned SP = M68K_REG_SP;
	M68K_REG_SP = M68K_MASK_OUT_BELOW_32(M68K_REG_SP + 4);
    return M68K_READ_32(SP);
}

// HELPER FUNCTIONS TO BE ABLE TO READ THE IMMEDIATE ADDRESSING VARIABLE
// OF A DATA REGISTER - APTLY NAMED DATA INCREMENT 
//
// THIS IS ACHIEVED BY PARSING THE HIGHEST ADDRESS OFFSET
// THATS READS THE IMMEDIATE VALUE - FOLLOWED BY PASSING THE EFFECTIVE ADDRESS
// TO THE CURRENT READ

unsigned int M68K_DI_8(void)
{
	unsigned EA = M68K_ADDRESS_HIGH + (S16)READ_IMM_16();
	return M68K_READ_8(EA);
}

unsigned int M68K_DI_16(void)
{
	unsigned EA = M68K_ADDRESS_HIGH + (S16)READ_IMM_16();
	return M68K_READ_16(EA);
}

unsigned int M68K_DI_32(void)
{
	unsigned EA = M68K_ADDRESS_HIGH + (S16)READ_IMM_16();
	return M68K_READ_32(EA);
} 

// LOAD THE BINARY FILE AS PER THE SIMULATOR
// THE IDEA FOR THIS IS TO DETERMINE THE SIZE OF THE FILE, THE MEMORY THAT NEEDS TO BE
// ALLOCATED AS SUCH

// AS WELL AS ACCOUNTING FOR THE START ADDRESSS OF THE PC

int LOAD_BINARY_FILE(const char* FILE_PATH, U32 LOAD_ADDR)
{
	const char* DOT = strrchr(FILE_PATH, '.');

	// DO WE HAVE A VALID EXTENSION?!
	// THIS IS A VERY RUDIMENTARY CHECKER
	if(!DOT || strcmp(DOT, ".bin") != 0)
	{
		printf("ERROR: FILE MUST HAVE EXTENSION .bin: %s\n", FILE_PATH);
		return -1;
	}

    FILE* FILE_PTR = fopen(FILE_PATH, "rb");
    if(!FILE_PTR)
    {
        printf("ERROR: COULD NOT OPEN FILE %s\n", FILE_PATH);
        return -1;
    }

    fseek(FILE_PTR, 0, SEEK_END);
    long FILE_SIZE = ftell(FILE_PTR);
    rewind(FILE_PTR);

    printf("LOADING BINARY FILE: %s AT 0x%04X\n", FILE_PATH, M68K_REG_PC);

    U8* BUFFER = malloc(FILE_SIZE);
    if(!BUFFER)
    {
        printf("ERROR: MEMORY ALLOCATION FAILED\n");
        fclose(FILE_PTR);
        return -1;
    }

    if(fread(BUFFER, 1, FILE_SIZE, FILE_PTR) != (size_t)FILE_SIZE)
	{
		printf("ERROR: COULD NOT READ FILE PROPERLY\n");
		free(BUFFER);
		fclose(FILE_PTR);
		return -1;
	}

	fclose(FILE_PTR);

    for(long INDEX = 0; INDEX < FILE_SIZE; INDEX++) 
	{
		 M68K_WRITE_8(LOAD_ADDR + INDEX, BUFFER[INDEX]); 
	}

    free(BUFFER);
    return FILE_SIZE;
}

#endif
#endif
