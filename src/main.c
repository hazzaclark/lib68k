/* COPYRIHGT (C) HARRY CLARK 2025*/
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "util.h"
#include "68K.h"
#include "68KCONF.h"
#include "68KOPCODE.h"
#include "68KSTD.h"
#include "68KMEM.h"

int main(void) 
{
    printf("====================================================\n");
    printf("HARRY CLARK - MOTOROLA 680x0 EMULATOR\n");
    printf("====================================================\n");
    
    M68K_INIT();
    M68K_SET_CPU_TYPE(M68K_CPU_000);

    printf("CPU SET TO TYPE: %d\n", M68K_CPU_000);

    SHOW_TRACE_STATUS();
    ENABLE_TRACE_FLAG(M68K_OPT_BASIC);

    MEMORY_MAP(0x00001000, 0x1000, true);

    printf("TESTING BASIC READ AND WRITES\n");

    U8 TEST_8 = 0xAA;
    M68K_WRITE_MEMORY_8(0x1000, TEST_8);
    U8 READ_8 = M68K_READ_MEMORY_8(0x1000);
    printf("8-BIT: WROTE: 0x%02X, READ: 0x%02X\n", TEST_8, READ_8);

    U16 TEST_16 = 0xBBCC;
    M68K_WRITE_MEMORY_16(0x1010, TEST_16);
    U16 READ_16 = M68K_READ_MEMORY_16(0x1010);
    printf("16-BIT: WROTE: 0x%04X, READ: 0x%04X\n", TEST_16, READ_16);

    U32 TEST_32 = 0x13400000;
    M68K_WRITE_MEMORY_32(0x1020, TEST_32);
    U32 READ_32 = M68K_READ_MEMORY_32(0x1020);
    printf("32-BIT: WROTE: 0x%08X, READ: 0x%08X\n", TEST_32, READ_32);

    return 0;
}
