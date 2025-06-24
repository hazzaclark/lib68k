/* COPYRIHGT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE ACTS AS AN EXPANSION OF THE PRE-REQUISTIES DEFINED IN THE MAIN CPU HEADER */
/* PROVIDING MOODULARISATION AND CROSS COMPILATION FOR OPCODE RELATED HANDLERS */

#ifndef M68K_OPCODE_BASE
#define M68K_OPCODE_BASE

/* NESTED INCLUDES */

#include "68K.h"
#include "68KCONF.h"
#include "common.h"

/* SYSTEM INCLUDES */

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(OPERAND_ACCESS)
    #define OPERAND_ACCESS
    #else
    #define OPERAND_ACCESS

        #undef USE_68K

        #define     M68K_DATA_HIGH          (CPU.DATA_REGISTER[(M68K_REG_IR >> 9) & 7])
        #define     M68K_DATA_LOW           (CPU.DATA_REGISTER[(M68K_REG_IR) & 7])

        #define     M68K_ADDRESS_HIGH       (CPU.ADDRESS_REGISTER[(M68K_REG_IR >> 9) & 7])
        #define     M68K_ADDRESS_LOW        (CPU.ADDRESS_REGISTER[(M68K_REG_IR) & 7])

#endif

#if defined(BUILD_OP_TABLE)
    #define BUILD_OP_TABLE
    #else
    #define BUILD_OP_TABLE

    #define         USE_OPCODE_DEBUG                M68K_OPT_OFF

    #define         OPCODE_ILLEGAL_MASK             (0x00000 << 16)
    #define         OPCODE_DEF_MASK                 (0xFF00 >> 0x0000)
    #define         OPCODE_BYTE_MASK                (0xF1F8 >> 0x0000)

    #define         OPCODE_BIT_MASK                 0xFF
    #define         OPCODE_MAX                      0x10000

    typedef struct OPCODE
    {
        char NAME;
        char SIZE;
        char BITS;
        void(*HANDLER)(void);
        unsigned MASK;
        char CYCLES;

        char PROCESS;
        char EA;

    } OPCODE;

    #ifndef USE_OPCODE_HANDLER_TABLE

    typedef struct OPCODE_HANDLER
    {
        void(*HANDLER)(void);
        unsigned MASK;
        unsigned MATCH;
        unsigned CYCLES;

    } OPCODE_HANDLER;

    #endif

    // THE WAY THAT I ORIGINALLY INTENDED FOR THIS MACRO TO
    // BE LAID OUT WAS THAT "REG" WOULD ENCOMPASS THE BASELINE
    // INITIALISER (HENCE WHY MOST OF THE DEFS HAVE "0" IN PLACE OF REG)

    // THE REST OF THE NAMES ARE INDICATIVE OF THE PRE-PROC FILL-INS

    #define M68K_MAKE_OPCODE(OP, SIZE, MODE, REG) \
    void OP##_##SIZE##_##MODE##_##REG(void)

    #define     OPCODE_NAME         OPCODE_BASE.NAME      
    #define     OPCODE_SIZE         OPCODE_BASE.SIZE
    #define     OPCODE_BITS         OPCODE_BASE.BITS
    #define     OPCODE_PROCESS      OPCODE_BASE.PROCESS
    #define     OPCODE_EA           OPCODE_BASE.EA
    #define     OPCODE_HANDLE       OPCODE_BASE.HANDLER
    #define     OPCODE_CYCLES       OPCODE_BASE.CYCLES

void M68K_OP_1010(void);
void M68K_OP_1111(void);
void M68K_OP_ILLEGAL_EX(void);

int EXTRACT_OPCODE(char* SRC, char* NAME, int* SIZE);
int CHECK_OPCODE_LENGTH(char* SRC, char* DEST, int MAX);
int GET_OP_CYCLES(OPCODE* OPCODE_BASE, int EA_MODE, int* CPU_TYPE);
OPCODE* FIND_OPCODE(char* NAME, int SIZE);

extern OPCODE OPCODE_BASE;
extern OPCODE_HANDLER OP_HANDLE;

extern OPCODE_HANDLER M68K_OPCODE_HANDLER_TABLE[OPCODE_MAX];
extern void(*M68K_OPCODE_JUMP_TABLE[0x10000])(void);

#endif
#endif
