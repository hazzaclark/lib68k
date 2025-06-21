// COPYRIGHT (C) HARRY CLARK 2025
// MOTOROLA 68000 STANDALONE EMULATION LIBRARY

// THIS FILE PERTAINS TOWARDS THE MODULARISATION OF THE MEMORY ENCOMPASSING THIS EMULATOR
// THIS IS JUST AN EXTENSION OF LIB68K_MEM, THAT OF WHICH ISOLATED THE LOGIC PERTAINING
// TOWARDS THIS EMULATOR TO BE ABLE TO FOSTER AN ACCURATE MEMORY SCHEME

// NESTED INCLUDES

#include "68KMEM.h"

static M68K_MEM_BUFFER MEM_BUFFERS[M68K_MAX_BUFFERS];
static unsigned MEM_NUM_BUFFERS;
static U32 MEM_FUNCTION_CALL;
U8 ENABLED_FLAGS = M68K_OPT_FLAGS;
bool TRACE_ENABLED;

void ENABLE_TRACE_FLAG(U8 FLAG) { ENABLED_FLAGS |= FLAG; }
void DISABLE_TRACE_FLAG(U8 FLAG) { ENABLED_FLAGS &= ~FLAG; }
bool IS_TRACE_ENABLED(U8 FLAG) { return (ENABLED_FLAGS & FLAG) == FLAG; }

void SHOW_TRACE_STATUS(void)
{
    printf("\nTRACE CONFIG:\n"); \
    printf("  BASIC:            %s\n", IS_TRACE_ENABLED(M68K_OPT_BASIC) ? "ENABLED" : "DISABLED"); \
    printf("  VERBOSE:          %s\n", (VERBOSE_TRACE_HOOK == M68K_OPT_ON && IS_TRACE_ENABLED(M68K_OPT_VERB)) ? "ENABLED" : "DISABLED"); \
    printf("  DEVICE TRACES:    %s\n", IS_TRACE_ENABLED(M68K_OPT_DEVICE) ? "ENABLED" : "DISABLED"); \
    printf("  T0 FLAG:          %s  (SHIFT: 0x%02X)\n", M68K_FLAG_T0 ? "ON" : "OFF", M68K_T0_SHIFT); \
    printf("  T1 FLAG:          %s (SHIFT: 0x%02X)\n", M68K_FLAG_T1 ? "ON" : "OFF", M68K_T1_SHIFT); \
    printf("  T0 ACTIVE:        %s\n", IS_TRACE_ENABLED(M68K_T0_SHIFT) ? "YES" : "NO"); \
    printf("  T1 ACTIVE:        %s\n", IS_TRACE_ENABLED(M68K_T1_SHIFT) ? "YES" : "NO"); \
    printf("\n");
}

void SHOW_MEMORY_MAPS(void)
{
    printf("\n%s MEMORY MAP(S):\n", M68K_CPU_STOPPED ? "AFTER EXEC" : "BEFORE EXEC");
    printf("--------------------------------------------------------------\n");
    printf("START        END         SIZE    STATE  READS   WRITES  ACCESS\n");
    printf("--------------------------------------------------------------\n");

    for(unsigned INDEX = 0; INDEX < MEM_NUM_BUFFERS; INDEX++)
    {
        M68K_MEM_BUFFER* BUF = &MEM_BUFFERS[INDEX];
        printf(" 0x%08X 0x%08X %6dKB    %s  %6u  %6u     %s\n",
                BUF->BASE,
                BUF->BASE + BUF->SIZE - 1,
                BUF->SIZE / 1024,
                BUF->WRITE ? "RW" : "RO",
                BUF->USAGE.READ_COUNT,
                BUF->USAGE.WRITE_COUNT,
                BUF->USAGE.ACCESSED ? "YES" : "NO");
    }

    printf("--------------------------------------------------------------\n");
}

// FIND THE CURRENTLY EXECUTED MEMORY BUFFER IN CONJUNCTION WITH 
// THE MEMORY RANGES THAT ARE BEING USED

// THIS WAY, WE ARE ABLE TO CONCLUSIVELY VALIDATE THE MEMORY BEING PASSED
// THROUGH AS PER THE INNATE FUNCTIONALITY OF THE OPCODES

static M68K_MEM_BUFFER* MEM_FIND(U32 ADDRESS)
{
    VERBOSE_TRACE("FOUND MEMORY: 0x%04X", ADDRESS);

    for(unsigned INDEX = 0; INDEX < MEM_NUM_BUFFERS; INDEX++)
    {
        // DEFINE THE BASE IN RELATION TO HOW MANY MAPS THERE ARE
        M68K_MEM_BUFFER* MEM_BASE = MEM_BUFFERS + INDEX;

        if((MEM_BASE == NULL) && MEM_BASE->BASE != ADDRESS)
        {
            VERBOSE_TRACE("NO BUFFER FOUND FOR ADDRESS: 0x%08x\n", ADDRESS);
            return NULL; 
        }

        // CAN WE ACCESS THE MEMORY BEING PASSED THROUGH
        // IF SO, VALIDATE WHAT TYPE IT IS AND RETURN

        if((MEM_BASE->BUFFER != NULL) && 
                (ADDRESS >= MEM_BASE->BASE) && 
                ((ADDRESS - MEM_BASE->BASE) < MEM_BASE->SIZE))
        {
            VERBOSE_TRACE("ACCESSED: 0x%04X [%s] IN BUFFER %u: 0x%08X - 0x%08X\n", 
                ADDRESS, 
                MEM_BASE->WRITE ? "RW" : "RO", 
                INDEX, 
                MEM_BASE->BASE, 
                MEM_BASE->BASE + MEM_BASE->SIZE - 1);

            return MEM_BASE;
        }
    }

    return NULL;
}

static U32 MEMORY_READ(U32 ADDRESS, U32 SIZE)
{
    VERBOSE_TRACE("READING ADDRESS: 0x%04X (SIZE = %d)", ADDRESS, SIZE);

    // BOUND CHECKS FOR INVALID ADDRESSING
    if(ADDRESS == M68K_MAX_ADDR_START && ADDRESS <= M68K_MAX_ADDR_END)
    {
        VERBOSE_TRACE("ATTEMPT TO READ FROM RESERVED ADDRESS RANGE: 0x%08X\n", ADDRESS);
        goto MALFORMED_READ;
    }

    // FIND THE ADDRESS AND IT'S RELEVANT SIZE IN ACCORDANCE WITH WHICH VALUE IS BEING PROC.
    M68K_MEM_BUFFER* MEM_BASE = MEM_FIND(ADDRESS);

    if(MEM_BASE != NULL)
    {
        // FIRST WE READ AND DETERMINE THE READ STATISTICS OF THE CURRENT MEMORY MAP BEING ALLOCATED
        
        MEM_BASE->USAGE.READ_COUNT++;
        MEM_BASE->USAGE.LAST_READ = ADDRESS;
        MEM_BASE->USAGE.ACCESSED = true;

        U32 OFFSET = (ADDRESS - MEM_BASE->BASE);
        if((OFFSET + (SIZE / 8)) > MEM_BASE->SIZE)
        {
            MEM_BASE->USAGE.VIOLATION++;
            VERBOSE_TRACE("READ OUT OF BOUNDS: OFFSET = %d, SIZE = %d, VIOLATION #%u\n", OFFSET, SIZE, MEM_BASE->USAGE.VIOLATION);
            goto MALFORMED_READ;
        }
        
        // THIS MEMORY POINTER WILL ALLOCATE ITSELF RELATIVE TO THE BUFFER
        // AS WELL AS THE BIT SHIFT OFFSET THAT IS PRESENT WITH THE RESPECTIVE BIT VALUE

        U8* MEM_PTR = MEM_BASE->BUFFER + OFFSET;
        U32 MEM_RETURN = 0;

        switch (SIZE)
        {
            case MEM_SIZE_32:
                MEM_RETURN = *MEM_PTR++;
                MEM_RETURN = (MEM_RETURN << 8) | *MEM_PTR++;
                MEM_RETURN = (MEM_RETURN << 8) | *MEM_PTR++;
                MEM_RETURN = (MEM_RETURN << 8) | *MEM_PTR;
                break;
                
            case MEM_SIZE_16:
                MEM_RETURN = *MEM_PTR++;
                MEM_RETURN = (MEM_RETURN << 8) | *MEM_PTR;
                break;
                
            case MEM_SIZE_8:
                MEM_RETURN = *MEM_PTR;
                break;
        }

        MEM_TRACE(MEM_READ, ADDRESS, SIZE, MEM_RETURN);
        return MEM_RETURN;
    }

MALFORMED_READ:
    fprintf(stderr, "BAD READ AT ADDRESS: 0x%08X\n", ADDRESS);
    MEM_TRACE(MEM_INVALID_READ, ADDRESS, SIZE, ~(U32)0);
    return 0;
}

static void MEMORY_WRITE(U32 ADDRESS, U32 SIZE, U32 VALUE)
{
    M68K_MEM_BUFFER* MEM_BASE = MEM_FIND(ADDRESS);

    if(MEM_BASE != NULL)
    {
        // BEFORE ANYTHING, WE NEED TO VALIDATE IF THE MEMORY MAP
        // IS EITHER RW OR JUST RO

        if(!MEM_BASE->WRITE) 
        {
            MEM_BASE->USAGE.VIOLATION++;
            VERBOSE_TRACE("WRITE ATTEMPT TO READ-ONLY MEMORY AT 0x%08x, VIOLATION #%u\n", ADDRESS, MEM_BASE->USAGE.VIOLATION);
            goto MALFORMED;
        }

         // FIRST WE READ AND DETERMINE THE WRITE STATISTICS OF THE CURRENT MEMORY MAP BEING ALLOCATED

        MEM_BASE->USAGE.WRITE_COUNT++;
        MEM_BASE->USAGE.LAST_WRITE = ADDRESS;
        MEM_BASE->USAGE.ACCESSED = true;

        U32 OFFSET = (ADDRESS - MEM_BASE->BASE);
        U32 BYTES = SIZE / 8;

        if((OFFSET + BYTES) > MEM_BASE->SIZE) 
        {
            VERBOSE_TRACE("WRITE OUT OF BOUNDS: OFFSET = %d, SIZE = %d\n", OFFSET, BYTES);
            goto MALFORMED;
        }

        U8* MEM_PTR = MEM_BASE->BUFFER + OFFSET;
        MEM_TRACE(MEM_WRITE, ADDRESS, SIZE, VALUE);

        switch (SIZE)
        {
            case MEM_SIZE_32:
                *MEM_PTR++ = (VALUE >> 24) & 0xFF;
                *MEM_PTR++ = (VALUE >> 16) & 0xFF;
                *MEM_PTR++ = (VALUE >> 8) & 0xFF;
                *MEM_PTR = VALUE & 0xFF;
                break;

            case MEM_SIZE_16:
                *MEM_PTR++ = (VALUE >> 8) & 0xFF;
                *MEM_PTR = VALUE & 0xFF;
                break;
            
            case MEM_SIZE_8:
                *MEM_PTR = VALUE & 0xFF;
                break;
        }
        return;
    }

MALFORMED:
    fprintf(stderr, "BAD WRITE AT ADDRESS: 0x%08X (SIZE: %d, VALUE: 0x%08X)\n", 
            ADDRESS, SIZE, VALUE);
    MEM_TRACE(MEM_INVALID_WRITE, ADDRESS, SIZE, VALUE);
}

void MEMORY_MAP(U32 BASE, U32 SIZE, bool WRITABLE) 
{
    if(MEM_NUM_BUFFERS >= M68K_MAX_BUFFERS) 
    {
        fprintf(stderr, "CANNOT MAP - TOO MANY BUFFERS\n");
        return;
    }

    M68K_MEM_BUFFER* BUF = &MEM_BUFFERS[MEM_NUM_BUFFERS++];
    BUF->BASE = BASE;
    BUF->SIZE = SIZE;
    BUF->WRITE = WRITABLE;
    BUF->BUFFER = malloc(SIZE);
    
    memset(BUF->BUFFER, 0, SIZE);
    memset(&BUF->USAGE, 0, sizeof(M68K_MEM_USAGE));
    BUF->USAGE.ACCESSED = false;

    MEM_MAP_TRACE(MEM_MAP, BUF->BASE, BUF->SIZE / 1024, BUF->BUFFER);
}

unsigned int M68K_READ_MEMORY_8(unsigned int ADDRESS) { return MEMORY_READ(ADDRESS, MEM_SIZE_8); }
unsigned int M68K_READ_MEMORY_16(unsigned int ADDRESS) { return MEMORY_READ(ADDRESS, MEM_SIZE_16); }
unsigned int M68K_READ_MEMORY_32(unsigned int ADDRESS) { return MEMORY_READ(ADDRESS, MEM_SIZE_32); }
void M68K_WRITE_MEMORY_8(unsigned int ADDRESS, uint8_t VALUE) { MEMORY_WRITE(ADDRESS, MEM_SIZE_8, VALUE); }
void M68K_WRITE_MEMORY_16(unsigned int ADDRESS, uint16_t VALUE) { MEMORY_WRITE(ADDRESS, MEM_SIZE_16, VALUE); }
void M68K_WRITE_MEMORY_32(unsigned int ADDRESS, uint32_t VALUE) { MEMORY_WRITE(ADDRESS, MEM_SIZE_32, VALUE); }
void MEM_SET_FC(unsigned int NEW_FUNC_CALL) { MEM_FUNCTION_CALL = NEW_FUNC_CALL; }
