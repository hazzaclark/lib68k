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

    SHOW_TRACE_STATUS();
    ENABLE_TRACE_FLAG(M68K_OPT_BASIC);
    
    M68K_INIT();
    M68K_EXEC(100);
    M68K_SET_CPU_TYPE(M68K_CPU_000);

    printf("CPU SET TO TYPE: %d\n", M68K_CPU_000);

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

    U16 IMM_16 = 0xFFFF;
    M68K_WRITE_MEMORY_16(0x1000, IMM_16);
    U16 IMM_READ_16 = M68K_READ_IMM_16(0x1000);
    printf("16-BIT IMM: WROTE: 0x%04X, READ: 0x%04X\n", IMM_16, IMM_READ_16);

    U32 IMM_32 = 0xFFFFFFFF;
    M68K_WRITE_MEMORY_32(0x1000, IMM_32);
    U32 IMM_READ_32 = M68K_READ_IMM_32(0x1000);
    printf("32-BIT IMM: WROTE: 0x%08X, READ: 0x%08X\n", IMM_32, IMM_READ_32);

    U16 IR = 0xFFFF;
    M68K_WRITE_16(0x1000, IR);
    U16 READ_16_2 = M68K_READ_16(0x1010);
    printf("16-BIT-IR: WROTE: 0x%04X, READ: 0x%04X\n", IR, READ_16_2);

    U16 IMM_HELPER = 0xBAAF;
    M68K_WRITE_16(0x1000, IMM_HELPER);

    U16 IMM_READ = READ_IMM_16();
    printf("16-BIT-IMM HELPER: WROTE: 0x%04X, READ: 0x%04X\n", IMM_HELPER, IMM_READ);

    return 0;
}
