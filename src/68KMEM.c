// COPYRIGHT (C) HARRY CLARK 2025
// MOTOROLA 68000 STANDALONE EMULATION LIBRARY

// THIS FILE PERTAINS TOWARDS THE MODULARISATION OF THE MEMORY ENCOMPASSING THIS EMULATOR
// THIS IS JUST AN EXTENSION OF LIB68K_MEM, THAT OF WHICH ISOLATED THE LOGIC PERTAINING
// TOWARDS THIS EMULATOR TO BE ABLE TO FOSTER AN ACCURATE MEMORY SCHEME

// NESTED INCLUDES

#include "68KMEM.h"

M68K_MEM_BUFFER MEM_BUFFERS[M68K_MAX_BUFFERS];
unsigned MEM_NUM_BUFFERS = 0;
U32 MEM_FUNCTION_CALL;

U8 ENABLED_FLAGS = M68K_OPT_FLAGS;
bool TRACE_ENABLED;

void ENABLE_TRACE_FLAG(U8 FLAG)
{
    ENABLED_FLAGS |= FLAG;
}

void DISABLE_TRACE_FLAG(U8 FLAG)
{
    ENABLED_FLAGS &= FLAG;
}

bool IS_TRACE_ENABLED(U8 FLAG)
{
    return (ENABLED_FLAGS & FLAG) != 0;
}

void SHOW_TRACE_STATUS(void)
{
    printf("\nTRACE CONFIG:\n");
    printf("  BASIC:   %s\n", IS_TRACE_ENABLED(M68K_OPT_BASIC) ? "ENABLED" : "DISABLED");
    printf("  VERBOSE: %s\n", IS_TRACE_ENABLED(M68K_OPT_VERB) ? "ENABLED" : "DISABLED");
    printf("  DEVICE TRACES:  %s\n", IS_TRACE_ENABLED(M68K_OPT_DEVICE) ? "ENABLED" : "DISABLED");
    printf("\n");
}

static M68K_MEM_BUFFER* MEM_FIND(uint32_t ADDRESS)
{
    VERBOSE_TRACE("FINDING ADDRESS 0x%08x", ADDRESS);

    for(unsigned INDEX = 0; INDEX < MEM_NUM_BUFFERS; INDEX++)
    {
        M68K_MEM_BUFFER* MEM_BASE = MEM_BUFFERS + INDEX;

        if((MEM_BASE->BUFFER != NULL) && (ADDRESS >= MEM_BASE->BASE) && ((ADDRESS - MEM_BASE->BASE) < MEM_BASE->SIZE))
        {
            VERBOSE_TRACE("FOUND BUFFER: %d: BASE = 0x%08x, SIZE = %d\n", INDEX, MEM_BASE->BASE, MEM_BASE->SIZE);
            return MEM_BASE;
        }
    }

    VERBOSE_TRACE("NO BUFFER FOUND FOR ADDRESS: 0x%08x\n", ADDRESS);
    return NULL;
}

static uint32_t MEMORY_READ(uint32_t ADDRESS, uint32_t SIZE)
{
    VERBOSE_TRACE("READING ADDRESS FROM 0x%08x (SIZE = %d)\n", ADDRESS, SIZE);

    // FIND THE ADDRESS AND IT'S RELEVANT SIZE IN ACCORDANCE WITH WHICH VALUE IS BEING PROC.
    M68K_MEM_BUFFER* MEM_BASE = MEM_FIND(ADDRESS);

    if(MEM_BASE != NULL)
    {
        uint32_t OFFSET = (ADDRESS - MEM_BASE->BASE);

        if(OFFSET > (MEM_BASE->SIZE - (SIZE / 8)))
        {
            VERBOSE_TRACE("READ OUT OF BOUNDS: OFFSET = %d, SIZE = %d\n", OFFSET, SIZE);
            goto MALFORMED_READ;
        }
        
        // THIS MEMORY POINTER WILL ALLOCATE ITSELF RELATIVE TO THE BUFFER
        // AS WELL AS THE BIT SHIFT OFFSET THAT IS PRESENT WITH THE RESPECTIVE BIT VALUE

        uint8_t* MEM_PTR = MEM_BASE->BUFFER + OFFSET;
        uint32_t MEM_RETURN = 0;

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
    MEM_TRACE(MEM_INVALID_READ, ADDRESS, SIZE, ~(uint32_t)0);
    return 0;
}

static void MEMORY_WRITE(uint32_t ADDRESS, uint32_t SIZE, uint32_t VALUE)
{
    M68K_MEM_BUFFER* MEM_BASE = MEM_FIND(ADDRESS);

    if(MEM_BASE != NULL)
    {
        uint32_t OFFSET = (ADDRESS - MEM_BASE->BASE);
        uint32_t BYTES = SIZE / 8;

        if(!MEM_BASE->WRITE) 
        {
            VERBOSE_TRACE("WRITE ATTEMPT TO READ-ONLY MEMORY AT 0x%08x\n", ADDRESS);
            goto MALFORMED;
        }

        if((OFFSET + BYTES) > MEM_BASE->SIZE) 
        {
            VERBOSE_TRACE("WRITE OUT OF BOUNDS: OFFSET = %d, SIZE = %d\n", OFFSET, BYTES);
            goto MALFORMED;
        }

        uint8_t* MEM_PTR = MEM_BASE->BUFFER + OFFSET;
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
    fprintf(stderr, "BAD WRITE AT ADDRESS: 0x%08X (SIZE: 0x%02X, VALUE: 0x%08X)\n", 
            ADDRESS, SIZE, VALUE);
    MEM_TRACE(MEM_INVALID_WRITE, ADDRESS, SIZE, VALUE);
}

void MEMORY_MAP(uint32_t BASE, uint32_t SIZE, bool WRITABLE) 
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
    BUF->BUFFER = calloc(SIZE, 1);
    memset(BUF->BUFFER, 0, SIZE);
    
    printf("MAPPED MEMORY: 0x%08x-0x%08X (%d BYTES)\n", 
           BASE, BASE + SIZE - 1, SIZE);
    MEM_TRACE(MEM_MAP, BASE, SIZE, 0);
}

unsigned int M68K_READ_MEMORY_8(unsigned int ADDRESS)
{
    return MEMORY_READ(ADDRESS, MEM_SIZE_8);
}

unsigned int M68K_READ_MEMORY_16(unsigned int ADDRESS)
{
    return MEMORY_READ(ADDRESS, MEM_SIZE_16);
}

unsigned int M68K_READ_MEMORY_32(unsigned int ADDRESS)
{
    return MEMORY_READ(ADDRESS, MEM_SIZE_32);
}

void M68K_WRITE_MEMORY_8(unsigned int ADDRESS, uint8_t VALUE) 
{
    MEMORY_WRITE(ADDRESS, MEM_SIZE_8, VALUE);
}

void M68K_WRITE_MEMORY_16(unsigned int ADDRESS, uint16_t VALUE) 
{
    MEMORY_WRITE(ADDRESS, MEM_SIZE_16, VALUE);
}

void M68K_WRITE_MEMORY_32(unsigned int ADDRESS, uint32_t VALUE) 
{
    MEMORY_WRITE(ADDRESS, MEM_SIZE_32, VALUE);
}

unsigned int M68K_READ_IMM_16(unsigned int ADDRESS)
{
    bool TRACE = TRACE_ENABLED;
    TRACE_ENABLED = false;

    unsigned int RESULT = M68K_READ_MEMORY_16(ADDRESS);
    TRACE_ENABLED = TRACE;

    return RESULT;
}

unsigned int M68K_READ_IMM_32(unsigned int ADDRESS)
{
    bool TRACE = TRACE_ENABLED;
    TRACE_ENABLED = false;

    unsigned int RESULT = M68K_READ_MEMORY_32(ADDRESS);
    TRACE_ENABLED = TRACE;

    return RESULT;
}

void MEM_SET_FC(unsigned int NEW_FUNC_CALL)
{
    MEM_FUNCTION_CALL = NEW_FUNC_CALL;
}
