/* COPYRIHGT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY SURROUNDING THE CONFIGURATION */
/* OF EACH RESPECTIVE CPU'S TYPE AND RELATIVE HANDLERS */ 

#ifndef M68K_CONFIG
#define M68K_CONFIG

/* NESTED INCLUDES */

#include "common.h"
#include "util.h"
#include "68K.h"
#include "68KMEM.h"

/*==============================================================================*/
/*							68000 DEBUG											*/
/*==============================================================================*/

	/* OPTIONS FOR IMMEDIATE ADDRESSING DIRECTIVES WHEN LOOKING */
	/* FOR CPU CALLBACKS */	

	#define M68K_OPT_OFF		0
	#define M68K_OPT_ON			1

	#ifndef 	USE_M68K_HOOKS
	#define 	USE_M68K_HOOKS

// DEBUG MESSAGE FOR ISOLATING WHICH OFFSET OF THE PC 
// CERTAIN JUMP CONDITIONS TAKE ON DURING EXECUTION

	#define		M68K_USE_SUPERVISOR			M68K_OPT_OFF
	#define 	M68K_CCR_LOGGING 			M68K_OPT_ON

	#define		M68K_S_FLAG_HOOK	M68K_OPT_ON
	#define		M68K_X_FLAG_HOOK	M68K_OPT_ON
	#define		M68K_Z_FLAG_HOOK	M68K_OPT_ON
	#define		M68K_V_FLAG_HOOK	M68K_OPT_ON
	#define		M68K_C_FLAG_HOOK	M68K_OPT_ON
	#define		M68K_N_FLAG_HOOK	M68K_OPT_ON

	#if M68K_CCR_LOGGING
	#if (M68K_S_FLAG_HOOK || \
		M68K_X_FLAG_HOOK || \
		M68K_Z_FLAG_HOOK || \
		M68K_V_FLAG_HOOK || \
		M68K_C_FLAG_HOOK || \
		M68K_N_FLAG_HOOK)
	
		#define M68K_PRINT_FLAG(COND, FLAG_VAR, NAME) \
			do { \
				if (COND == M68K_OPT_ON) \
					printf(NAME ":%d ", ((FLAG_VAR) & 1)); \
			} while(0)
	
		#define M68K_CCR_HOOK() \
			do { \
				printf("CCR: "); \
				M68K_PRINT_FLAG(M68K_S_FLAG_HOOK, M68K_FLAG_S, "S"); \
				M68K_PRINT_FLAG(M68K_X_FLAG_HOOK, M68K_FLAG_X, "X"); \
				M68K_PRINT_FLAG(M68K_N_FLAG_HOOK, M68K_FLAG_N, "N"); \
				M68K_PRINT_FLAG(M68K_Z_FLAG_HOOK, M68K_FLAG_Z, "Z"); \
				M68K_PRINT_FLAG(M68K_V_FLAG_HOOK, M68K_FLAG_V, "V"); \
				M68K_PRINT_FLAG(M68K_C_FLAG_HOOK, M68K_FLAG_C, "C"); \
				printf("\n"); \
			} while(0)
	
	#else
		#define M68K_CCR_HOOK() ((void)0)
		#endif

	// EXTRA #ELSE CLAUSE SO THAT THE COMPILER WILL SHUT UP

	#else
	#define M68K_CCR_HOOK() ((void)0)
	#endif

	#define 	M68K_JUMP_HOOK 		M68K_OPT_ON
	#define		M68K_RTS_HOOK		M68K_OPT_ON
	#define		M68K_RESET_HOOK		M68K_OPT_ON
	#define		M68K_ILLEGAL_HOOK	M68K_OPT_ON
	#define		M68K_LEA_HOOK		M68K_OPT_ON
	#define		M68K_ADDR_HOOK		M68K_OPT_ON
	#define		M68K_EA_HOOK		M68K_OPT_ON
	#define		M68K_DISP_HOOK		M68K_OPT_ON

	#if M68K_JUMP_HOOK == M68K_OPT_ON
    #define M68K_BASE_JUMP_HOOK(ADDR, FROM_ADDR) \
        do { \
            printf("[JUMP TRACE] TO PC: 0x%04X FROM PC: 0x%04X\n", (ADDR), (FROM_ADDR)); \
        } while(0)
	#endif

	#if M68K_RTS_HOOK
    #define M68K_BASE_RTS_HOOK(FROM_ADDR) \
        do { \
            printf("[RTS] 0x%04X\n", (FROM_ADDR)); \
        } while(0)
	#else
    	#define M68K_BASE_RTS_HOOK(FROM_ADDR) ((void)0)
	#endif

	#if M68K_RESET_HOOK
    #define M68K_BASE_RES_HOOK(T0, T1, PC, SP) \
        do { \
            printf("RESET WITH TRACE LEVEL (T0: %d, T1: %d) -> CURRENT PC: 0x%04X -> CURRENT SP: 0x%08X\n\n", \
                  (T0), (T1), (PC), (SP)); \
        } while(0)
	#else
    	#define M68K_BASE_RES_HOOK(T0, T1, PC, SP) ((void)0)
	#endif

	#if M68K_ILLEGAL_HOOK
    #define M68K_BASE_ILL_HOOK(PC, IR) \
        do { \
            printf("ILLEGAL INSTRUCTION FOUND AT PC -> (0x%04X) -> CURRENT OPCODE: (0x%04X)\n", \
                  (PC), (IR)); \
        } while(0)
	#else
    	#define M68K_BASE_ILL_HOOK(PC, IR) ((void)0)
	#endif

	// FOR THE PRINTING OF THE EA OF LEA
	// IT SKIPS THE OPCODE TO READ THE NEXT 4 BYTES

	#if M68K_LEA_HOOK
	#define M68K_BASE_LEA_HOOK(REG_ARRAY) \
		do { \
			int REG_NUM = (M68K_REG_IR >> 9) & 7; \
			int EA = READ_IMM_16(); \
			M68K_REG_PC -= 2;		\
			printf("LEA FOUND: $%x -> REGISTER A%d\n", \
				EA, REG_NUM); \
		} while(0)
	#else
		#define M68K_BASE_LEA_HOOK(REG_ARRAY) ((void)0)
	#endif

	#if M68K_ADDR_HOOK
	#define M68K_BASE_ADDRESS_HOOK(REG_ARRAY) \
		do { \
			int REG_NUM = (M68K_REG_IR >> 9) & 7; \
			printf("ADDRESS FOUND WITH REGISTER: %d\n", \
				REG_NUM); \
		} while(0)
	#else
		#define M68K_BASE_ADDRESS_HOOK(REG_ARRAY) ((void)0)
	#endif

	#if M68K_EA_HOOK
    #define M68K_EA_PRINT_HOOK(REG_ARRAY) \
        do { \
            int EA_MODE = M68K_GET_EA_MODE(M68K_REG_IR); \
            int EA_REG = M68K_GET_EA_REG(M68K_REG_IR); \
            printf("EA MODE: %d, EA REGISTER: %d\n", EA_MODE, EA_REG); \
        } while(0)
	#else
    	#define M68K_EA_PRINT_HOOK(REG_ARRAY) ((void)0)
	#endif

	// WE CAN LEVERAGE READING BASED ON PC
	// AS THE WILL USUALLY ADVANCE IN CONJUNCTION WITH DISPLACMENT EITHER WAY

	// NEGATE THE ADVANCE FROM THE IMMEDIATE READ AFTERWARDS
	
	#if M68K_DISP_HOOK
    #define M68K_DISP_PRINT_HOOK() \
        do { \
            U8 EA_MODE = M68K_GET_EA_MODE(M68K_REG_IR); \
            U8 EA_REG = M68K_GET_EA_REG(M68K_REG_IR); \
            S16 DISP = (S16)M68K_GET_DISP(READ_IMM_16()); \
            printf("EA MODE: %d, EA REGISTER: %d\n", EA_MODE, EA_REG); \
            printf("DISPLACEMENT: %d (0x%04X)\n", DISP, (U16)DISP); \
            M68K_REG_PC -= 2; \
        } while(0)
	#else
    	#define M68K_DISP_PRINT_HOOK() ((void)0)
	#endif

	#endif

#define 	M68K_ADD_CYCLES(VALUE)				M68K_MASTER_CYC += (VALUE)
#define		M68K_USE_CYCLES(VALUE)				M68K_MASTER_CYC += (VALUE)
#define		M68K_SET_CYCLES(VALUE)				M68K_MASTER_CYC = VALUE
#define		M68K_GET_CYCLES()					M68K_MASTER_CYC
#define		M68K_ALL_CYCLES()					M68K_MASTER_CYC %= M68K_CYCLE[M68K_REG_IR]
#define		M68K_USE_MASTER_CYCLES(VALUE)		M68K_MASTER_CYC += (VALUE)

#define 	M68K_MASK_OUT_ABOVE_8(A)            ((A) & 0xFF)
#define 	M68K_MASK_OUT_ABOVE_16(A)           ((A) & 0xFFFF)
#define 	M68K_MASK_OUT_ABOVE_32(A)           ((A) & 0xFFFFFFFF)

#define 	M68K_MASK_OUT_BELOW_8(A)  			((A) & ~0xFF)
#define 	M68K_MASK_OUT_BELOW_16(A)  			((A) & ~0xFFFF)
#define 	M68K_MASK_OUT_BELOW_32(A)  			((A) & ~0xFFFFFFFF)

#define		M68K_RETURN_ADDRESS(ADDRESS)        (ADDRESS & 0xFFFFFFFFFF)

#define		M68K_BIT_SHIFT_8(VALUE)				((VALUE) >> 7) & 1
#define		M68K_BIT_SHIFT_16(VALUE)			((VALUE) >> 15) & 1
#define		M68K_BIT_SHIFT_32(VALUE)			((VALUE) >> 31) & 1

#define		M68K_BIT_SHIFT_N_8(VALUE)			(VALUE)
#define		M68K_BIT_SHIFT_N_16(VALUE)			((VALUE) >> 8)
#define		M68K_BIT_SHIFT_N_24(VALUE)			((VALUE) >> 16)
#define		M68K_BIT_SHIFT_N_32(VALUE)			((VALUE) >> 24)

#define 	M68K_FLAG_V_8(SRC, DEST, RESULT)  		((SRC ^ RESULT) & (DEST ^ RESULT))
#define 	M68K_FLAG_V_16(SRC, DEST, RESULT) 		(((SRC ^ RESULT) & (DEST ^ RESULT)) >> 8)
#define 	M68K_FLAG_V_32(SRC, DEST, RESULT) 		(((SRC ^ RESULT) & (DEST ^ RESULT)) >> 24)

// SEPERATE HOOK FOR ACCESSING A PRE-SUPPOSED STATE WITHIN
// THE CONTEXT OF THE EMULATOR

// THIS CAN VARY BASED ON PRE-DEFINED BEHAVIOURS WITHIN
// THE CONDITIONS BY WHICH WE ARE ABLE TO HOOK AND ACCESS THESE CONDITIONS

// ERGO, WHEN THE CPU IS HALTED/PAUSED, OR IF IT IS RESET AND NEEDS TO
// JUMP BACK TO AN INITIAL COUROUTINE

			#define		M68K_STATE_HOOK		M68K_OPT_ON

			#if M68K_STATE_HOOK

			#define	M68K_SAVE_STATE(PARAM, SIZE)						\
				printf("SAVE -> [%u]+%zu ", BUFFER, (UNK)(SIZE));		\
				memcpy(&STATE[BUFFER], PARAM, SIZE);					\
				BUFFER += SIZE;

			
			#define	M68K_LOAD_STATE(PARAM, SIZE)						\
				printf("LOAD -> [%u]+%zu ", BUFFER, (UNK)(SIZE));		\
				memcpy(PARAM, &STATE[BUFFER], SIZE);					\
				BUFFER += SIZE;

			#endif

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

extern unsigned int M68K_GET_EA_EXT(unsigned BASE);

extern unsigned int M68K_POST_DEC_16(void);
extern unsigned int M68K_POST_DEC_32(void);

extern void M68K_PUSH_SP(unsigned VALUE);
extern unsigned int M68K_PULL_SR(void);
extern unsigned int M68K_PULL_SP(void);

extern unsigned int M68K_GET_IX_8(void);
extern unsigned int M68K_GET_IX_16(void);
extern unsigned int M68K_GET_IX_32(void);

extern U8 M68K_VECTOR_TABLE[5][256];

#endif
