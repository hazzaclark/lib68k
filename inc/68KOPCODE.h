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

#if defined(OPERAND_ACCESS)
    #define OPERAND_ACCESS
    #else
    #define OPERAND_ACCESS

        #undef USE_68K

        #define     M68K_DATA_HIGH          (M68K_REG_D[(M68K_REG_IR >> 9) & 7])
        #define     M68K_DATA_LOW           (M68K_REG_D[(M68K_REG_IR) & 7])

        #define     M68K_ADDRESS_HIGH       (M68K_REG_A[(M68K_REG_IR >> 9) & 7])
        #define     M68K_ADDRESS_LOW        (M68K_REG_A[(M68K_REG_IR) & 7])

#endif

#if defined(BUILD_OP_TABLE)
    #define BUILD_OP_TABLE
    #else
    #define BUILD_OP_TABLE

    #define         USE_OPCODE_DEBUG                M68K_OPT_OFF
    #define         OPCODE_MAX                      0x10000

    typedef struct OPCODE_HANDLER
    {
        void(*HANDLER)(void);
        unsigned MASK;
        unsigned MATCH;
        unsigned CYCLES;

    } OPCODE_HANDLER;

    // ALL ENCOMPASSING MACRO FOR CREATING OPCODES
    // CONCATENATES THE RESPECTIVE TYPES ACCORDINGLY
    //
    // THE PARAMETERS THAT ARE CONCATENATED ARE
    // VERBATIM AS THEY APPEAR IN THEIR INSTRUCTION
    // FORMAT IN RELEVANT DOCUMENTATION
    //
    // 17/10/25 - SOME OPCODES IN MY IMPL
    // HAVE RIDICULOUS VAR NAMES FOR CONCATENATION
    // ADDED VA_ARGS TO MITIGATE THIS  

    #define     M68K_MAKE_OPCODE(OP, SIZE, MODE, ...) \
    void OP##_##SIZE##_##MODE##_##__VA_ARGS__(void)
    
    #define     M68K_EXCEPTION(...) \
    void __VA_ARGS__(void) 

extern OPCODE_HANDLER M68K_OPCODE_HANDLER_TABLE[OPCODE_MAX];
extern void(*M68K_OPCODE_JUMP_TABLE[0x10000])(void);

#endif
#endif
