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

#define         M68K_MAX_BUFFERS                5
#define         M68K_OPT_BASIC                  (1 << 0)
#define         M68K_OPT_VERB                   (1 << 1)
#define         M68K_OPT_DEVICE                 (1 << 2)

#define         M68K_MAX_ADDR_START             0x000000
#define         M68K_MAX_MEMORY_SIZE            0xFFFFFF
#define         M68K_MAX_ADDR_END               (M68K_MAX_ADDR_START + M68K_MAX_MEMORY_SIZE)

#define         M68K_T0_SHIFT                   (1 << 3)
#define         M68K_T1_SHIFT                   (1 << 4)

#define         M68K_OPT_FLAGS                  (M68K_OPT_BASIC | M68K_OPT_VERB | M68K_OPT_DEVICE)

typedef enum
{
    MEM_READ = 'R',
    MEM_WRITE = 'W',
    MEM_INVALID_READ = '!',
    MEM_INVALID_WRITE = '?',
    MEM_MAP = 'M',
    MEM_UNMAP = 'U',
    MEM_MOVE = 'O', 
    MEM_ERR = 'E'

} M68K_MEM_OP;

typedef enum
{
    MEM_SIZE_8 = 8,
    MEM_SIZE_16 = 16,
    MEM_SIZE_32 = 32

} M68K_MEM_SIZE;

typedef enum
{   
    MEM_OK,
    MEM_ERR_BOUNDS,
    MEM_ERR_READONLY,
    MEM_ERR_UNMAPPED,
    MEM_ERR_BUS,
    MEM_ERR_BUFER,
    MEM_ERR_SIZE,
    MEM_ERR_RESERVED,
    MEM_ERR_OVERFLOW,
    MEM_ERR_BAD_READ,
    MEM_ERR_BAD_WRITE

} M68K_MEM_ERROR;

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
    U32 END;
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
#define         MEM_MAP_TRACE_HOOK              M68K_OPT_ON
#define         VERBOSE_TRACE_HOOK              M68K_OPT_OFF
#define         JUMP_HOOK                       M68K_OPT_ON
#define         PHASE_HOOK                      M68K_OPT_ON

#if MEM_TRACE_HOOK == M68K_OPT_ON
    #define MEM_TRACE(OP, ADDR, SIZE, VAL) \
        do { \
            if (IS_TRACE_ENABLED(M68K_OPT_BASIC) && CHECK_TRACE_CONDITION()) \
                printf("[TRACE] %c -> ADDR:0x%08X SIZE:%d VALUE:0x%04X\n", \
                      (char)(OP), (ADDR), (SIZE), (VAL)); \
        } while(0)
#else
    #define MEM_TRACE(OP, ADDR, SIZE, VAL) ((void)0)
#endif

#if MEM_MAP_TRACE_HOOK == M68K_OPT_ON
    #define MEM_MAP_TRACE(OP, BASE, END, SIZE, VAL) \
    do { \
        if (IS_TRACE_ENABLED(M68K_OPT_BASIC) && CHECK_TRACE_CONDITION()) \
            printf("[TRACE] %c -> START:0x%08X END:0x%08X SIZE:%d%s\n", \
                  (char)(OP), (BASE), (END), FORMAT_SIZE(SIZE), FORMAT_UNIT(SIZE)); \
    } while(0)
#else
    #define MEM_MAP_TRACE(OP, BASE, END, SIZE, UNIT, VAL) ((void)0)
#endif

#define MEM_ERROR(OP, ERROR_CODE, SIZE, MSG, ...) \
    do { \
        if (IS_TRACE_ENABLED(M68K_OPT_VERB) && CHECK_TRACE_CONDITION()) \
            printf("[ERROR] %c -> %-18s [SIZE: %d]: " MSG "\n", \
                (char)(OP), M68K_MEM_ERR[ERROR_CODE], \
                (int)(SIZE), ##__VA_ARGS__); \
    } while(0)

#define VERBOSE_TRACE(MSG, ...) \
    do { \
        if (VERBOSE_TRACE_HOOK == M68K_OPT_ON && IS_TRACE_ENABLED(M68K_OPT_VERB) && CHECK_TRACE_CONDITION()) \
            printf("[VERBOSE] " MSG "\n", ##__VA_ARGS__); \
    } while(0)

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

void MEMORY_MAP(U32 BASE, U32 END, bool WRITABLE);
void SHOW_MEMORY_MAPS(void);
void SHOW_TRACE_STATUS(void);

extern U8 ENABLED_FLAGS;
