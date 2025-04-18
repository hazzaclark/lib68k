/* COPYRIHGT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY SURROUNDING THE CONFIGURATION */
/* OF EACH RESPECTIVE CPU'S TYPE AND RELATIVE HANDLERS */ 

#ifndef M68K_CONFIG
#define M68K_CONFIG

/* NESTED INCLUDES */

#include "common.h"
#include "68KMEM.h"

/*===============================================================================*/
/*							68000 DEBUG											 */
/*===============================================================================*/

/* MISCALLANEOUS TYPES FOR A LINE EXCEPTION HANDLERS */
/* THIS WILL BE USED IN DOCUMENTING CYCLE ACCURATE EXCEPTIONS AT RUNTIME */

#if M68K_LOG_ENABLE
	extern FILE* M68K_LOG_FILEHANDLE
	#define M68K_DO_LOG(A) 			if(M68K_LOG_FILEHANDLE) 
	#if M68K_LOG_1010_1111_A_LINE
		#define M68K_DO_LOG(A) 		if(M68K_LOG_FILEHANDLE) fprintf(stdout, A)
	#else
		#define M68K_DO_LOG_EMU(A)
	#endif
#else
	#define M68K_DO_LOG(A)
	#define M68K_DO_LOG_EMU(A)

	/* OPTIONS FOR IMMEDIATE ADDRESSING DIRECTIVES WHEN LOOKING */
	/* FOR CPU CALLBACKS */	

	#define M68K_OPT_OFF				0
	#define M68K_OPT_ON					1

#endif

// ADDED THIS CONFIG HERE TO ALLOW FOR PROPER HOOK EMULATION
// THE IDEA IS TO ALLOW FOR A BETTER MEANS OF BEING ABLE TO DETERMINE HOW AND WHEN
// INSTRUCTIONS JUMP AND BRANCH

#if M68K_EMU_INSTR_HOOK
	#if M68K_EMU_INSTR_HOOK == M68K_OPT_ON
		#define M68K_BASE_INSTR_HOOK
	#else
		#define M68K_BASE_INSTR_HOOK(PC)	M68K_INSTR_HOOK(PC)
	#endif
#else
	#define	M68K_BASE_INSTR_HOOK(PC)
#endif

#define			M68K_EMU_PREF		M68K_OPT_OFF

#ifdef USE_CYCLES
#define USE_CYCLES
#else

#define 	M68K_ADD_CYCLES(VALUE)				M68K_MASTER_CYC += (VALUE)
#define		M68K_USE_CYCLES(VALUE)				M68K_CYC_REMAIN -= (VALUE)
#define		M68K_SET_CYCLES(VALUE)				M68K_MASTER_CYC = VALUE
#define		M68K_GET_CYCLES()					M68K_MASTER_CYC
#define		M68K_ALL_CYCLES()					M68K_MASTER_CYC %= M68K_CYCLE[M68K_REG_IR]

#define		M68K_USE_MASTER_CYCLES(VALUE)		CPU.MASTER_CYCLES += (VALUE)

#endif

#define 	M68K_MASK_OUT_ABOVE_8(A)            ((A) & 0xFF)
#define 	M68K_MASK_OUT_ABOVE_16(A)           ((A) & 0xFFFF)
#define 	M68K_MASK_OUT_ABOVE_32(A)           ((A) & 0xFFFFFFFF)
#define     M68K_RETURN_ADDRESS(ADDRESS)        (ADDRESS & 0xFFFFFFFFFF)

#define		M68K_BIT_SHIFT_8(VALUE)			((VALUE) >> 7) & 1
#define		M68K_BIT_SHIFT_16(VALUE)			((VALUE) >> 15) & 1
#define		M68K_BIT_SHIFT_32(VALUE)			((VALUE) >> 31) & 1


#define		M68K_MAX_RAM						0xFFFF
#define		M68K_MAX_ROM						0xFFFFFFFF
#define 	M68K_MAX_MEMORY_BUFFER_SIZE 		0x100000

#define 	M68K_FUNCTION_CODE_USER_DATA          	1
#define 	M68K_FUNCTION_CODE_USER_PROGRAM       	2
#define 	M68K_FUNCTION_CODE_SUPERVISOR_DATA    	5
#define 	M68K_FUNCTION_CODE_SUPERVISOR_PROGRAM 	6
#define 	M68K_FUNCTION_CODE_CPU_SPACE          	7

#define		M68K_SET_FC(A)					MEM_SET_FC(A)
#define		M68K_ADDRESS_LINE(A)			((A)&M68K_ADDRESS_MASK)

/*==============================================================================*/
/*							68000 CALLBACKS										*/
/*==============================================================================*/

extern U8 M68K_VECTOR_TABLE[5][256];
int M68K_SET_INT_CALLBACK(int LEVEL);
void M68K_DEFAULT_INSTR_CALLBACK(void);
void M68K_SET_FUNC_CALLBACK(void (*CALLBACK)(void));
void M68K_SET_INSTR_CALLBACK(unsigned CALLBACK, unsigned PC);
U32 M68K_FETCH_LONG();

/*===============================================================================*/
/*							68000 READ AND WRITE							     */
/*===============================================================================*/

extern unsigned int READ_IMM_8(void);
extern unsigned int READ_IMM_16(void);
extern unsigned int READ_IMM_32(void);

unsigned int CTRL_READ_BYTE(unsigned int ADDRESS);
unsigned int CTRL_READ_WORD(unsigned int ADDRESS);
void CTRL_WRITE_BYTE(unsigned int ADDRESS, unsigned int DATA);
void CTRL_WRITE_WORD(unsigned int ADDRESS, unsigned int DATA);

void M68K_SET_CPU_TYPE(unsigned TYPE);
int M68K_CYCLES_RUN(void);
int M68K_CYCLES_REMAINING(void);
void INITIALIZE_MEMORY();

int LOAD_BINARY_FILE(const char* FILE_PATH, U32 LOAD_ADDR);

extern unsigned int M68K_DI_8(void);
extern unsigned int M68K_DI_16(void);
extern unsigned int M68K_DI_32(void); 

extern unsigned int M68K_POST_DEC_32(void);

extern void M68K_PUSH_SP(unsigned VALUE);
extern unsigned int M68K_PULL_SP(void);


#endif
