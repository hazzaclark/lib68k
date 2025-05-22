/* COPYRIHGT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDSS THE MAIN FUNCTIONALITY SURROUNDING THE UTILITY */
/* OF THE CPU AND IT'S SUBSEQUENT DIRECTIVES AND DEFINES */

/* ANY AND ALL OF THE CORRESPONDING DEFINES AND DECLARATIONS CAN BE */
/* ATTRIBUTED TO THE INFROMATION FOUND HERE: https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf  */ 

#ifndef M68K_BASE
#define M68K_BASE

/* NESTED INCLUDES */

#include "common.h"

/* SYSTEM INCLUDES */

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#if defined(USE_68K)
    #define USE_68K
        #else
        #define USE_68K

    #if defined(USE_CONFIG)
    #define USE_CONFIG
        #else
    #define USE_CONFIG
    #include "68KCONF.h"
        #endif

        #if defined(USE_OPCODE)
        #define USE_OPCODE
            #else
        #define USE_OPCODE
        #include "68KOPCODE.h"
        #endif

            #if defined(USE_MEM)
            #define USE_MEM
                #else
            #define USE_MEM
            #include "68KMEM.h"
            #endif
#endif

/*===============================================================================*/
/*							68000 MAIN CPU FUNCTIONALIY							 */
/*===============================================================================*/

#define         M68K_ADDRESS_WIDTH_8            0xFF
#define         M68K_ADDRESS_WIDTH_16           0xFFFF
#define         M68K_ADDRESS_WIDTH_32           0xFFFFFFFF
#define         M68K_SYMBOL_WIDTH               ' '
#define         M68K_ADDRESS_IA                 (0x8000000 << 0xDFFFFFF)
#define         M68K_BASE_BITMASK               256

/*===============================================================================*/
/*                          68000 MEMORY UTILITY MACROS                          */
/*                      FOR READING THE BYTE ORDER OF MEMORY                     */
/*===============================================================================*/  

#define     M68K_READ_8(VALUE)                              M68K_READ_MEMORY_8(VALUE)
#define     M68K_READ_16(VALUE)                             M68K_READ_MEMORY_16(VALUE)
#define     M68K_READ_32(VALUE)                             M68K_READ_MEMORY_32(VALUE)
#define     M68K_WRITE_8(ADDRESS, VALUE)                    M68K_WRITE_MEMORY_8(ADDRESS, VALUE)
#define     M68K_WRITE_16(ADDRESS, VALUE)                   M68K_WRITE_MEMORY_16(ADDRESS, VALUE)
#define     M68K_WRITE_32(ADDRESS, VALUE)                   M68K_WRITE_MEMORY_32(ADDRESS, VALUE)
                                                     

#define         M68K_EA()                                   M68K_ADDRESS_HIGH
#define         M68K_EA_INCR_BYTE()                         (M68K_ADDRESS_HIGH++)
#define         M68K_EA_INCR_WORD()                         ((M68K_ADDRESS_HIGH += 2) - 2)
#define         M68K_EA_INCR_WORD_LONG()                    ((M68K_ADDRESS_HIGH += 4) - 4)
#define         M68K_EA_DECR_BYTE()                         (M68K_ADDRESS_HIGH--)
#define         M68K_EA_DECR_WORD()                         (M68K_ADDRESS_HIGH -= 2)
#define         M68K_EA_DECR_WORD_LONG()                    (M68K_ADDRESS_HIGH -= 4)

#define         M68K_GET_CCR()                             (((M68K_FLAG_X & 0x100) >> 4) << 8)
#define         M68K_GET_SR()                              (((M68K_FLAG_X & 0x100) >> 1) << 13)

#define         M68K_BIT_0(VALUE)                                   ((VALUE) & 0x00000001)
#define         M68K_BIT_1(VALUE)                                   ((VALUE) & 0x00000002)
#define         M68K_BIT_2(VALUE)                                   ((VALUE) & 0x00000004)
#define         M68K_BIT_3(VALUE)                                   ((VALUE) & 0x00000008)
#define         M68K_BIT_4(VALUE)                                   ((VALUE) & 0x00000010)
#define         M68K_BIT_5(VALUE)                                   ((VALUE) & 0x00000020)
#define         M68K_BIT_6(VALUE)                                   ((VALUE) & 0x00000040)
#define         M68K_BIT_7(VALUE)                                   ((VALUE) & 0x00000080)
#define         M68K_BIT_8(VALUE)                                   ((VALUE) & 0x00000100)
#define         M68K_BIT_9(VALUE)                                   ((VALUE) & 0x00000200)
#define         M68K_BIT_A(VALUE)                                   ((VALUE) & 0x00000400)
#define         M68K_BIT_B(VALUE)                                   ((VALUE) & 0x00000800)
#define         M68K_BIT_C(VALUE)                                   ((VALUE) & 0x00001000)
#define         M68K_BIT_D(VALUE)                                   ((VALUE) & 0x00004000)
#define         M68K_BIT_E(VALUE)                                   ((VALUE) & 0x00002000)
#define         M68K_BIT_F(VALUE)                                   ((VALUE) & 0x00008000)
#define         M68K_BIT_10(VALUE)                                  ((VALUE) & 0x00010000)
#define         M68K_BIT_11(VALUE)                                  ((VALUE) & 0x00020000)
#define         M68K_BIT_12(VALUE)                                  ((VALUE) & 0x00040000)
#define         M68K_BIT_13(VALUE)                                  ((VALUE) & 0x00080000)
#define         M68K_BIT_14(VALUE)                                  ((VALUE) & 0x00100000)
#define         M68K_BIT_15(VALUE)                                  ((VALUE) & 0x00200000)
#define         M68K_BIT_16(VALUE)                                  ((VALUE) & 0x00400000)
#define         M68K_BIT_17(VALUE)                                  ((VALUE) & 0x00800000)
#define         M68K_BIT_18(VALUE)                                  ((VALUE) & 0x01000000)
#define         M68K_BIT_19(VALUE)                                  ((VALUE) & 0x02000000)
#define         M68K_BIT_1A(VALUE)                                  ((VALUE) & 0x04000000)
#define         M68K_BIT_1B(VALUE)                                  ((VALUE) & 0x08000000)
#define         M68K_BIT_1C(VALUE)                                  ((VALUE) & 0x10000000)
#define         M68K_BIT_1D(VALUE)                                  ((VALUE) & 0x20000000)
#define         M68K_BIT_1E(VALUE)                                  ((VALUE) & 0x40000000)
#define         M68K_BIT_1F(VALUE)                                  ((VALUE) & 0x80000000)

#define         M68K_GET_EA_MODE(IR) \
                ((M68K_BIT_5(IR) ? 4 : 0) | \
                (M68K_BIT_4(IR) ? 2 : 0) | \
                (M68K_BIT_3(IR) ? 1 : 0))

#define         M68K_GET_EA_REG(IR) \
                ((M68K_BIT_2(IR) ? 4 : 0) | \
                (M68K_BIT_1(IR) ? 2 : 0) | \
                (M68K_BIT_0(IR) ? 1 : 0))

#define         M68K_GET_EA_VALUE(IR) \
                ((M68K_BIT_2(IR) ? 4 : 0) | \
                (M68K_BIT_1(IR) ? 2 : 0) | \
                (M68K_BIT_0(IR) ? 1 : 0))

#define         M68K_LOW_NIBBLE(VALUE)
#define         M68K_HIGH_NIBBLE(VALUE)             ((VALUE & 0x8000) ? 1 : 0)

#define         M68K_IMM_NIBBLE()                   \
            ((U32)READ_IMM_32() << 31) | READ_IMM_32()

#define         M68K_LEA_AW()               READ_IMM_16()
#define         M68K_LEA_AL()               READ_IMM_32()

#define         M68K_LEA_AI()                   M68K_ADDRESS_LOW

                // POST INCREMENT

#define         M68K_LEA_PI_8()                 (M68K_ADDRESS_LOW++)
#define         M68K_LEA_PI_16()    	        ((M68K_ADDRESS_LOW += 2) - 2)
#define         M68K_LEA_PI_32()    	        ((M68K_ADDRESS_LOW += 4) - 4)

                // PRE DECREMENT

#define         M68K_LEA_PD_8()                 (--M68K_ADDRESS_LOW)
#define         M68K_LEA_PD_16()                ((M68K_ADDRESS_LOW -= 2))
#define         M68K_LEA_PD_32()                ((M68K_ADDRESS_LOW -= 4))

#define         M68K_LEA_AW_16()            M68K_LEA_AW()
#define         M68K_LWA_AW_32()            M68K_LEA_AW()

#define         M68K_LEA_AL_16()            M68K_LEA_AL()
#define         M68K_LEA_AL_32()            M68K_LEA_AL()

#define         M68K_CPU_000                0
#define         M68K_CPU_010                1
#define         M68K_CPU_020                2
#define         M68K_CPU_030                3
#define         M68K_CPU_040                4

#define         M68K_IRQ_NONE               0
#define         M68K_IRQ_1                  1
#define         M68K_IRQ_2                  2
#define         M68K_IRQ_3                  3
#define         M68K_IRQ_4                  4
#define         M68K_IRQ_5                  5
#define         M68K_IRQ_6                  6
#define         M68K_IRQ_7                  7

#define			M68K_LOW_BITMASK		8*7
#define			M68K_MID_BITMASK		16*7
#define			M68K_HIGH_BITMASK		24*7
#define 		M68K_MAX_BITMASK		32*7

#define 	    M68K_EXCEPTION_RESET                         0
#define 	    M68K_EXCEPTION_BUS_ERROR                     2 
#define 	    M68K_EXCEPTION_ADDRESS_ERROR                 3
#define 	    M68K_EXCEPTION_ILLEGAL_INSTRUCTION           4
#define 	    M68K_EXCEPTION_ZERO_DIVIDE                   5
#define 	    M68K_EXCEPTION_CHK                           6
#define 	    M68K_EXCEPTION_TRAPV                         7
#define 	    M68K_EXCEPTION_PRIVILEGE_VIOLATION           8
#define 	    M68K_EXCEPTION_TRACE                         9   
#define 	    M68K_EXCEPTION_1010                          10
#define 	    M68K_EXCEPTION_1111                          11
#define 	    M68K_EXCEPTION_FORMAT_ERROR                  14
#define 	    M68K_EXCEPTION_UNINITIALIZED_INTERRUPT       15
#define 	    M68K_EXCEPTION_SPURIOUS_INTERRUPT            24
#define 	    M68K_EXCEPTION_INTERRUPT_AUTOVECTOR          24
#define 	    M68K_EXCEPTION_TRAP_BASE                     32

#define         M68K_EMULATE_TRACE_MODE                             0
#define         M68K_EMULATE_TRACE_MODE_OFF                         1

// ENABLE AND DISABLE TRACE FLAG EMULATION
// THIS WILL PROVE TO BE FRUITFUL TO PROVIDE GREATER CLARITY ON MEMORY READINGS

#if M68K_EMULATE_TRACE_MODE
    #define     M68K_TRACE_1()          TRACE_MODE = M68K_FLAG_T1
    #define     M68K_TRACE_0()          TRACE_MODE |= M68K_FLAG_T0
    #define     M68K_TRACE_CLEAR()      TRACE_MODE = 0

#else
    #undef      M68K_EMULATE_TRACE_MODE_OFF 
    #define     M68K_TRACE_1()
    #define     M68K_TRACE_0()
    #define     M68K_TRACE_CLEAR()
#endif

typedef struct CPU_68K
{
    unsigned int PC;
    unsigned int* INSTRUCTION_CYCLES;
    unsigned int REMAINING_CYCLES;
    unsigned int CYCLE_RATE;
    unsigned int RESET_CYCLES;
    unsigned int* CYCLE_EXCEPTION;
    unsigned int CYCLE_END;

    unsigned(*MEMORY_DATA);
    unsigned(*MEMORY_ADDRESS);
    unsigned(*MEMORY_POINTER);

    unsigned(*LOW_ADDR);
    unsigned(*HIGH_ADDR);
    void(*USER_DATA);

    unsigned int STOPPED;

    U16 STATUS_REGISTER;
    U32 INDEX_REGISTER;
    U32 REGISTER_BASE[16];
    U32 DATA_REGISTER[8];
    U32 ADDRESS_REGISTER[8];
    U32 PREVIOUS_PC;
    U32 JUMP_TARGET;
    U32 STACK_POINTER;
    U32 INTERRUPT_SP;
    U32 MASTER_SP;
    U32 USER_STACK;
    U32 ADDRESS_STACK_POINTER;
    U32 INSTRUCTION_REGISTER;
    U32 SOURCE_FUNCTION_COUNTER;
    U32 DEST_FUNCTION_COUNTER;
    U32 VBR;
    U32 FPR[8];
    U32 FPIAR;
    U32 FPCR;
    U32 FPSR;
    U32 CACHE_CONTROL;
    U32 CACHE_ADDRESS;

    char INSTRUCTION_MODE;
    char TRACE_FLAG;

    unsigned int PREVIOUS_DATA;
    unsigned int PREVIOUS_ADDRESS;
    unsigned int ADDRESS_RT_CHECK;
    unsigned char ERROR_ADDRESS;
    unsigned char ERROR_WRITE_MODE;
    unsigned char ERROR_PC;
    UNK* ERROR_JUMP;

    S32(*INTERRUPT_CALLBACK)(unsigned INTERRUPT);
    S32(*RESET_INTERRUPT)(void);
    S32(*CPU_FUNC_CALLBACK)(unsigned FUNCTION);
    unsigned int* INT_LEVEL;

    unsigned int CPU_STOPPED;

    unsigned S_FLAG;
    unsigned X_FLAG;
    unsigned N_FLAG;
    unsigned V_FLAG;
    unsigned Z_FLAG;
    unsigned C_FLAG;
    unsigned M_FLAG;

    unsigned T0_FLAG;
    unsigned T1_FLAG;

    int(*INT_ACK_CALLBACK)(int VALUE);
    void(*RESET_CALLBACK)(void);
    void(*PC_CHANGED_CALLBACK)(unsigned* NEW_PC);
    void(*SET_FC_CALLBACK)(void);
    void(*INSTR_HOOK)(unsigned PC);

    unsigned int ADDRESS_MASK;
    unsigned int SR_MASK;

    unsigned int RESET_MODE;

    unsigned PREFETCH_ADDRESS;
    unsigned PREFETCH_MODE;
    unsigned PREFETCH_DATA;

    signed int MASTER_CYCLES;
    signed int REFRESH_CYCLES;

} CPU_68K;

extern CPU_68K CPU;

typedef enum CPU_68K_REGS
{
	M68K_REG_TYPE,
    M68K_D0 = 0,    
    M68K_D1 = 1,
    M68K_D2 = 2,
    M68K_D3 = 3,
    M68K_D4 = 4,
    M68K_D5 = 5,
    M68K_D6 = 6,
    M68K_D7 = 7,
    M68K_A0 = 8,    
    M68K_A1 = 9,
    M68K_A2 = 10,
    M68K_A3 = 11,
    M68K_A4 = 12,
    M68K_A5 = 13,
    M68K_A6 = 14,
    M68K_A7 = 15,
    M68K_PC,    
    M68K_SR,    
    M68K_SP,    
    M68K_USP,   
    M68K_ISP,
    M68K_IR, 
    M68K_SFC,
    M68K_VBR,
    M68K_DFC,
    M68K_CACR,
    M68K_CAAR

} CPU_68K_REGS;

typedef enum CPU_68K_FLAGS 
{
    FLAG_S,
    FLAG_X,
    FLAG_Z,
    FLAG_N,
    FLAG_C,
    FLAG_V,
    FLAG_T0,
    FLAG_T1,
    FLAG_M

} CPU_68K_FLAGS;

#define         M68K_REG_BASE           CPU.REGISTER_BASE
#define 		M68K_REG_DA             CPU.DATA_REGISTER
#define			M68K_REG_D              CPU.DATA_REGISTER
#define			M68K_REG_A              CPU.ADDRESS_REGISTER
#define			M68K_REG_SR             CPU.STATUS_REGISTER
#define			M68K_REG_PPC            CPU.PREVIOUS_PC
#define         M68K_REG_JMP_TARG       CPU.JUMP_TARGET
#define			M68K_REG_PC             CPU.PC
#define			M68K_REG_SP             CPU.STACK_POINTER
#define			M68K_REG_USP            CPU.USER_STACK
#define			M68K_REG_ISP            CPU.INTERRUPT_SP
#define			M68K_REG_MSP            CPU.MASTER_SP
#define			M68K_REG_VBR            CPU.VBR
#define			M68K_REG_SFC            CPU.SOURCE_FUNCTION_COUNTER
#define			M68K_REG_DFC            CPU.DEST_FUNCTION_COUNTER
#define			M68K_REG_CACR           CPU.CACHE_CONTROL
#define			M68K_REG_CAAR           CPU.CACHE_ADDRESS
#define			M68K_REG_IR             CPU.INDEX_REGISTER
#define 		M68K_REG_FPR            CPU.FPR
#define			M68K_REG_FPCR           CPU.FPCR
#define			M68K_REG_FPSR           CPU.FPSR
#define			M68K_REG_FPIAR          CPU.FPIA

#define  		M68K_FLAG_T0            CPU.T0_FLAG
#define			M68K_FLAG_T1            CPU.T1_FLAG
#define			M68K_FLAG_S             CPU.S_FLAG
#define			M68K_FLAG_M             CPU.M_FLAG
#define			M68K_FLAG_X             CPU.X_FLAG
#define			M68K_FLAG_N             CPU.N_FLAG
#define			M68K_FLAG_Z             CPU.Z_FLAG
#define			M68K_FLAG_V             CPU.V_FLAG
#define			M68K_FLAG_C             CPU.C_FLAG
#define			M68K_FLAG_INT_LVL       CPU.INT_LEVEL
#define			M68K_CPU_STOPPED        CPU.CPU_STOPPED

#define         M68K_CYC_REMAIN         CPU.REMAINING_CYCLES
#define			M68K_CYC_EXCE           CPU.CYCLE_EXCEPTION
#define 		M68K_CYCLE              CPU.INSTRUCTION_CYCLES
#define         M68K_RESET_LVL          CPU.RESET_MODE
#define         M68K_RESET_CYCLES       CPU.RESET_CYCLES

#define         M68K_INT_ACK            CPU.INT_ACK_CALLBACK
#define         M68K_RESET_ACK          CPU.RESET_CALLBACK
#define         M68K_PC_CHANGED         CPU.PC_CHANGED_CALLBACK
#define         M68K_SET_FC_ACK         CPU.SET_FC_CALLBACK
#define         M68K_INSTR_HOOK         CPU.INSTR_HOOK

#define         M68K_ADDRESS_MASK       CPU.ADDRESS_MASK
#define         M68K_SR_MASK            CPU.SR_MASK

#define         M68K_MEMORY_DATA        CPU.MEMORY_DATA
#define         M68K_MEMORY_ADDRESS     CPU.MEMORY_ADDRESS
#define         M68K_MEMORY_PTR         CPU.MEMORY_POINTER

#define         M68K_PRV_DATA           CPU.PREVIOUS_DATA
#define         M68K_PRV_ADDR           CPU.PREVIOUS_ADDRESS
#define         M68K_TRA_ADDR           CPU.ADDRESS_RT_CHECK

#define         M68K_PREF_ADDRESS       CPU.PREFETCH_ADDRESS
#define         M68K_PREF_MODE          CPU.PREFETCH_MODE
#define         M68K_PREF_DATA          CPU.PREFETCH_DATA

#define         M68K_MASTER_CYC         CPU.MASTER_CYCLES
#define         M68K_REFRESH_CYC        CPU.REFRESH_CYCLES

#define         M68K_COND_FLAG_Z()      M68K_FLAG_Z
#define         M68K_COND_FLAG_V()      M68K_FLAG_V
#define         M68K_COND_FLAG_C()      M68K_FLAG_C
#define         M68K_COND_FLAG_X()      M68K_FLAG_X
#define         M68K_COND_FLAG_Z()      M68K_FLAG_Z
#define         M68K_COND_FLAG_M()      M68K_FLAG_M
#define         M68K_COND_FLAG_S()      M68K_FLAG_S

/*==============================================================================*/
/*                          68000 MAIN CPU FUNCTIONALIY                         */
/*==============================================================================*/

void INITIALISE_68K_CYCLES();
unsigned int M68K_GET_REGISTERS(struct CPU_68K* CPU, int REGISTER);
void M68K_SET_REGISTERS(unsigned int REGISTER, unsigned int VALUE);

void M68K_INIT(void);
void M68K_MEM_INIT(void);
int M68K_EXEC(int CYCLES);
void M68K_JUMP();
void M68K_JUMP_VECTOR(unsigned VECTOR);
void M68K_SET_SR_IRQ(unsigned VALUE);
void M68K_PULSE_RESET(void);
void M68K_PULSE_HALT(void);
void M68K_CLEAR_HALT(void);

void M68K_SET_S_FLAG(unsigned VALUE);
void M68K_SET_CCR(unsigned VALUE);
void M68K_SET_SR(unsigned VALUE);

void M68K_BUILD_OPCODE_TABLE(void);

void M68K_BRANCH_8(unsigned OFFSET);
void M68K_BRANCH_16(unsigned OFFSET);
void M68K_BRANCH_32(unsigned OFFSET);

/*==============================================================================*/
/*                                  68000 MISC.                                 */
/*==============================================================================*/

extern int M68K_REMAINING_CYCLES;
extern unsigned int M68K_ADDRESSING_SPACE; 
extern unsigned char CYCLE_RANGE[0x10000];
extern unsigned CPU_TYPE;

#endif
