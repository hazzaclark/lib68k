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
        U16 MASK;
        U16 MATCH;
        unsigned CYCLES;

    } OPCODE_HANDLER;

    typedef struct EXCEPTION_HANDLER
    {
        U8 VECTOR;
        U8 CYCLES;
        void(*HANDLER)(void);

    } EXCEPTION_HANDLER;

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
    extern void(*M68K_EXCEPTION_JUMP_TABLE[256])(void);
    extern void M68K_BUILD_EXCEPTION_TABLE(void);

#endif

/* ============================================================ */
/*               68000 CYCLE ACCURACY DECLARATIONS              */
/* ============================================================ */

#if defined(USE_EA_CYCLES)
    #define USE_EA_CYCLES
#else
    #define USE_EA_CYCLES

    #include <stdint.h>

    // DEFINE THE BASIS FOR EA MODE FIELDS AND THEIR RESPECTIVE BITS 
    // THAT WILL BE EXTRACTED TO ACCESS THE MODE ITSELF
    
    #define         M68K_EA_DN              0x0
    #define         M68K_EA_AN              0x1
    #define         M68K_EA_AN_IND          0x2
    #define         M68K_EA_AN_POST         0x3
    #define         M68K_EA_AN_PRE          0x4
    #define         M68K_EA_AN_DISP         0x5
    #define         M68K_EA_AN_IDX          0x6
    #define         M68K_EA_EXT             0x7

    #define         M68K_EA_EXT_ABS_W       0x0
    #define         M68K_EA_EXT_ABS_L       0x1
    #define         M68K_EA_EXT_PC          0x2
    #define         M68K_EA_EXT_PC_IDX      0x3
    #define         M68K_EA_EXT_IMM         0x4

    // CREATE PRE-COMPUTED LOOKUP TABLES WITH THE RESPECTIVE VALUES REQUIRED
    // PER EACH OF THE EA MODES
    //
    // FOR EA MODES WITH EXTENSION, IT WILL JUST BE ASSUMED THAT GIVEN 
    // THE ENCOMPASSING LENGTH OF THE WORD, THAT ALL OF THE CYCLES WILL JUST
    // BE ONE HOLISTIC VALUE, RATHER THAN EIGHT SLOTS REPRESENTING 
    // THE REQUIRED BITS CONVERSELY
    //
    // THE FOLLOWING ARE THE DETERMINISTIC OPCODES VALUE
    // WHEN MODE AND XN ARE EXTRACTED FROM THE OPCODE WORD
    //
    // SEE CYCLE ACCURACY: https://wiki.neogeodev.org/index.php?title=68k_instructions_timings

    #define         M68K_EA_FILL_SLOTS(VALUE)           {(VALUE), (VALUE), (VALUE), (VALUE), (VALUE), (VALUE), (VALUE), (VALUE)}

    static const U8 M68K_EA_CYCLES_BW[8][8] = 
    {
        [M68K_EA_DN]                =  M68K_EA_FILL_SLOTS(0),
        [M68K_EA_AN]                =  M68K_EA_FILL_SLOTS(0),
        [M68K_EA_AN_IND]            =  M68K_EA_FILL_SLOTS(4),
        [M68K_EA_AN_POST]           =  M68K_EA_FILL_SLOTS(4),
        [M68K_EA_AN_PRE]            =  M68K_EA_FILL_SLOTS(6),
        [M68K_EA_AN_DISP]           =  M68K_EA_FILL_SLOTS(8),
        [M68K_EA_AN_IDX]            =  M68K_EA_FILL_SLOTS(10),
        
        [M68K_EA_EXT]   =
        {
            [M68K_EA_EXT_ABS_W]     =  8,
            [M68K_EA_EXT_ABS_L]     =  12,
            [M68K_EA_EXT_PC]        =  8,
            [M68K_EA_EXT_PC_IDX]    =  10,
            [M68K_EA_EXT_IMM]       =  4,
        },
    };

    static const U8 M68K_EA_CYCLES_L[8][8] =
    {
        [M68K_EA_DN]                =  M68K_EA_FILL_SLOTS(0),
        [M68K_EA_AN]                =  M68K_EA_FILL_SLOTS(0),
        [M68K_EA_AN_IND]            =  M68K_EA_FILL_SLOTS(8),
        [M68K_EA_AN_POST]           =  M68K_EA_FILL_SLOTS(8),
        [M68K_EA_AN_PRE]            =  M68K_EA_FILL_SLOTS(10),
        [M68K_EA_AN_DISP]           =  M68K_EA_FILL_SLOTS(12),
        [M68K_EA_AN_IDX]            =  M68K_EA_FILL_SLOTS(14),
        
        [M68K_EA_EXT]   =
        {
            [M68K_EA_EXT_ABS_W]     =  12,
            [M68K_EA_EXT_ABS_L]     =  16,
            [M68K_EA_EXT_PC]        =  12,
            [M68K_EA_EXT_PC_IDX]    =  14,
            [M68K_EA_EXT_IMM]       =  8,
        },
    };

    // EVALUATIVE MACROS WHICH ACT AS DIRECT LOOKUP FOR A GIVEN OPCODE WORD 
    // USED WHEN THE MODE AND XN COME FROM AN ACTUAL OPCODE WORD

    #define         M68K_EA_CYCLES_BW(MODE, XN)             \
                ((unsigned)M68K_EA_CYCLES_BW[(MODE)][(XN)])

     #define         M68K_EA_CYCLES_L(MODE, XN)             \
                ((unsigned)M68K_EA_CYCLES_L[(MODE)][(XN)])


    // DETERMINES THE EVALUATIVE SIZE OF A PROVIDED OPCODE WORD
    // TO VALIDATE THE CORRECT CYCLE COUNTS PER ITS SIZE
    #define         M68K_EA_CYCLE_EVAL(SIZE, MODE, XN)      \
            ((SIZE) == 4 ? M68K_EA_CYCLES_L((MODE), (XN)) : M68K_EA_CYCLES_BW((MODE), (XN)))

#endif
#endif
