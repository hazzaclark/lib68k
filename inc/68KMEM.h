// COPYRIGHT (C) HARRY CLARK 2025
// MOTOROLA 68000 STANDALONE EMULATION LIBRARY

// THIS FILE PERTAINS TOWARDS THE MODULARISATION OF THE MEMORY ENCOMPASSING THIS EMULATOR
// THIS IS JUST AN EXTENSION OF LIB68K_MEM, THAT OF WHICH ISOLATED THE LOGIC PERTAINING
// TOWARDS THIS EMULATOR TO BE ABLE TO FOSTER AN ACCURATE MEMORY SCHEME

#ifndef M68K_MEM
#define M68K_MEM

// NESTED INCLUDES

#include "68K.h"
#include "68KCONF.h"

#define         M68K_MAX_BUFFERS                16
#define         M68K_OPT_BASIC                  (1 << 0)
#define         M68K_OPT_VERB                   (1 << 1)
#define         M68K_OPT_DEVICE                 (1 << 2)

#define         M68K_MAX_ADDR_START             0xFFFFFFF0
#define         M68K_MAX_ADDR_END               0xFFFFFFFF

#define         M68K_T0_SHIFT                   (1 << 3)
#define         M68K_T1_SHIFT                   (1 << 4)

#define         M68K_OPT_FLAGS                  (M68K_OPT_BASIC | M68K_OPT_VERB)

typedef enum
{
    MEM_READ = 'R',
    MEM_WRITE = 'W',
    MEM_INVALID_READ = 'r',
    MEM_INVALID_WRITE = 'w',
    MEM_MAP = 'M',
    MEM_UNMAP = 'U',
    MEM_MOVE = 'O', 

} M68K_MEM_OP;

typedef enum
{
    MEM_SIZE_8 = 8,
    MEM_SIZE_16 = 16,
    MEM_SIZE_32 = 32

} M68K_MEM_SIZE;

typedef struct
{
    U32 READ_COUNT;
    U32 WRITE_COUNT;
    U32 LAST_READ;
    U32 LAST_WRITE;
    U32 VIOLATION;
    bool ACCESSED;

} M68K_MEM_USAGE;

typedef struct 
{
    U32 BASE;
    U32 SIZE;
    U8* BUFFER;
    bool WRITE;
    M68K_MEM_USAGE USAGE;

} M68K_MEM_BUFFER;

#define         CHECK_TRACE_CONDITION()         (IS_TRACE_ENABLED(M68K_T0_SHIFT) || IS_TRACE_ENABLED(M68K_T1_SHIFT))

/////////////////////////////////////////////////////
//              TRACE CONTROL MACROS
/////////////////////////////////////////////////////

#define         MEM_TRACE_HOOK                  M68K_OPT_ON
#define         VERBOSE_TRACE_HOOK              M68K_OPT_ON
#define         JUMP_HOOK                       M68K_OPT_ON
#define         PHASE_HOOK                      M68K_OPT_ON

#if MEM_TRACE_HOOK == M68K_OPT_OFF
    #define MEM_TRACE(OP, ADDR, SIZE, VAL) \
        do { \
            if (IS_TRACE_ENABLED(M68K_OPT_BASIC) && CHECK_TRACE_CONDITION()) \
                printf("[TRACE] %c ADDR:0x%04X SIZE:%d VALUE:0x%04X\n", \
                      (char)(OP), (ADDR), (SIZE), (VAL)); \
        } while(0)
#else
    #define MEM_TRACE(OP, ADDR, SIZE, VAL) ((void)0)
#endif

#if VERBOSE_TRACE_HOOK == M68K_OPT_OFF
    #define VERBOSE_TRACE(MSG, ...) \
        do { \
            if (IS_TRACE_ENABLED(M68K_OPT_VERB)) \
                printf("[VERBOSE] " MSG "\n", ##__VA_ARGS__); \
        } while(0)
#else
    #define VERBOSE_TRACE(MSG, ...) ((void)0)
#endif

#define SET_TRACE_FLAGS(T0, T1) \
        do {    \
            M68K_FLAG_T0 = (T0); \
            M68K_FLAG_T1 = (T1); \
            (T0) ? ENABLE_TRACE_FLAG(M68K_T0_SHIFT) : DISABLE_TRACE_FLAG(M68K_T0_SHIFT); \
            (T1) ? ENABLE_TRACE_FLAG(M68K_T1_SHIFT) : DISABLE_TRACE_FLAG(M68K_T1_SHIFT); \
    } while(0)
#endif

unsigned int M68K_READ_MEMORY_8(unsigned int ADDRESS);
unsigned int M68K_READ_MEMORY_16(unsigned int ADDRESS);
unsigned int M68K_READ_MEMORY_32(unsigned int ADDRESS);

void M68K_WRITE_MEMORY_8(unsigned int ADDRESS, U8 VALUE);
void M68K_WRITE_MEMORY_16(unsigned int ADDRESS, U16 VALUE);
void M68K_WRITE_MEMORY_32(unsigned int ADDRESS, U32 VALUE);

void ENABLE_TRACE_FLAG(U8 FLAG);
void DISABLE_TRACE_FLAG(U8 FLAG);
bool IS_TRACE_ENABLED(U8 FLAG);

void MEMORY_MAP(U32 BASE, U32 SIZE, bool WRITABLE);
void SHOW_MEMORY_MAPS(void);
void SHOW_TRACE_STATUS(void);

void MEM_SET_FC(unsigned int NEW_FUNC_CALL);

extern U8 ENABLED_FLAGS;
