/* COPYRIHGT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY SURROUNDING THE CONFIGURATION */
/* OF EACH RESPECTIVE CPU'S TYPE AND RELATIVE HANDLERS */ 

#ifndef M68K_CONFIG
#define M68K_CONFIG

/* NESTED INCLUDES */

#include "common.h"

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

#ifdef USE_CYCLES
#define USE_CYCLES
#else

#define 	M68K_ADD_CYCLES(VALUE)				M68K_CYC_REMAIN += (*VALUE)
#define		M68K_USE_CYCLES(VALUE)				M68K_CYC_REMAIN -= (VALUE)
#define		M68K_SET_CYCLES(VALUE)				M68K_CYC_REMAIN = VALUE
#define		M68K_GET_CYCLES()					M68K_CYC_REMAIN
#define		M68K_ALL_CYCLES()					M68K_CYC_REMAIN %= M68K_CYCLE[M68K_REG_IR]

#endif

#define	USE_BE 		M68K_OPT_OFF
#define	USE_LE 		M68K_OPT_ON

#define 	M68K_MASK_OUT_ABOVE_8(A)            ((A) & 0xFF)
#define 	M68K_MASK_OUT_ABOVE_16(A)           ((A) & 0xFFFF)
#define 	M68K_MASK_OUT_ABOVE_32(A)           ((A) & 0xFFFFFFF)
#define     M68K_RETURN_ADDRESS(ADDRESS)        (ADDRESS & 0xFFFFFFFFFF)

#define		M68K_MAX_RAM						0xFF
#define		M68K_MAX_ROM						0xFFFF
#define 	M68K_MAX_MEMORY_BUFFER_SIZE 		0x10000

#define 	M68K_FUNCTION_CODE_USER_DATA          	1
#define 	M68K_FUNCTION_CODE_USER_PROGRAM       	2
#define 	M68K_FUNCTION_CODE_SUPERVISOR_DATA    	5
#define 	M68K_FUNCTION_CODE_SUPERVISOR_PROGRAM 	6
#define 	M68K_FUNCTION_CODE_CPU_SPACE          	7

#define		M68K_SET_FC(A)					M68K_SET_FUNC_CALLBACK(A)
#define		M68K_ADDRESS_LINE(A)			((A)&M68K_ADDRESS_MASK)

/*===============================================================================*/
/*							68000 CALLBACKS						     			 */
/*===============================================================================*/

extern U8 M68K_VECTOR_TABLE[5][256];
int M68K_SET_INT_CALLBACK(int LEVEL);
void M68K_DEFAULT_INSTR_CALLBACK(void);
void M68K_SET_FUNC_CALLBACK(void (*CALLBACK)(void));
void M68K_SET_INSTR_CALLBACK(unsigned CALLBACK, unsigned PC);
U32 M68K_FETCH_LONG();

/*===============================================================================*/
/*							68000 READ AND WRITE							     */
/*===============================================================================*/


U8 M68K_READ_8(U32 ADDRESS);
U16 M68K_READ_16(U32 ADDRESS);
U32 M68K_READ_32(U32 ADDRESS);

void M68K_WRITE_8(unsigned int ADDRESS, unsigned int DATA);
void M68K_WRITE_16(unsigned int ADDRESS, unsigned int DATA);
void M68K_WRITE_32(unsigned int ADDRESS, unsigned int DATA);

unsigned int CTRL_READ_BYTE(unsigned int ADDRESS);
unsigned int CTRL_READ_WORD(unsigned int ADDRESS);
void CTRL_WRITE_BYTE(unsigned int ADDRESS, unsigned int DATA);
void CTRL_WRITE_WORD(unsigned int ADDRESS, unsigned int DATA);

void M68K_SET_CPU_TYPE(unsigned TYPE);
int M68K_CYCLES_RUN(void);
int M68K_CYCLES_REMAINING(void);
void INITIALIZE_MEMORY();


#endif
