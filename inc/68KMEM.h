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

#define     M68K_MAX_BUFFERS            10
#define     M68K_OPT_BASIC              (1 << 0)
#define     M68K_OPT_VERB               (1 << 1)
#define     M68K_OPT_DEVICE             (1 << 2)

#define     M68K_OPT_FLAGS              (M68K_OPT_BASIC | M68K_OPT_VERB)

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
    U32 BASE;
    U32 SIZE;
    U8* BUFFER;
    bool WRITE;

} M68K_MEM_BUFFER;

void ENABLE_TRACE_FLAG(U8 FLAG);
void DISABLE_TRACE_FLAG(U8 FLAG);
bool IS_TRACE_ENABLED(U8 FLAG);

void MEMORY_MAP(U32 BASE, U32 SIZE, bool WRITABLE);
void SHOW_TRACE_STATUS(void);

/////////////////////////////////////////////////////
//              TRACE CONTROL MACROS
/////////////////////////////////////////////////////

#if DEFAULT_TRACE_FLAGS & TRACE_BASIC
    #define MEM_TRACE(OP, ADDR, SIZE, VAL) \
        do { \
            if (IS_TRACE_ENABLED(TRACE_BASIC)) \
                printf("[TRACE] %c ADDR:0x%08x SIZE:%d VALUE:0x%08x\n", \
                      (char)(OP), (ADDR), (SIZE), (VAL)); \
        } while(0)
#else
    #define MEM_TRACE(OP, ADDR, SIZE, VAL) ((void)0)
#endif

#if DEFAULT_TRACE_FLAGS & TRACE_VERBOSE
    #define VERBOSE_TRACE(MSG, ...) \
        do { \
            if (IS_TRACE_ENABLED(TRACE_VERBOSE)) \
                printf("[VERBOSE] %s:%d " MSG "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        } while(0)
#else
    #define VERBOSE_TRACE(MSG, ...) ((void)0)
#endif

unsigned int M68K_READ_MEMORY_8(unsigned int ADDRESS);
unsigned int M68K_READ_MEMORY_16(unsigned int ADDRESS);
unsigned int M68K_READ_MEMORY_32(unsigned int ADDRESS);

void M68K_WRITE_MEMORY_8(unsigned int ADDRESS, U8 VALUE);
void M68K_WRITE_MEMORY_16(unsigned int ADDRESS, U16 VALUE);
void M68K_WRITE_MEMORY_32(unsigned int ADDRESS, U32 VALUE);


#endif
