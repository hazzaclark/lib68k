/* COPYRIHGT (C) HARRY CLARK 2025*/
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "util.h"
#include "68K.h"
#include "68KOPCODE.h"
#include "68KSTD.h"

int main(void) 
{
    printf("====================================================\n");
    printf("HARRY CLARK - MOTOROLA 680x0 EMULATOR\n");
    printf("====================================================\n");

    printf("Initialising 68000\n");

    M68K_INIT();

    // TESTING MEMORY READS

    U8 ADDRESS_8 = 0x3F;
    U8 VALUE_8 = M68K_READ_8(ADDRESS_8);
    
    printf("M68K READ 8 VALUE FROM ADDRESS 0x%02X: 0x%02X\n", ADDRESS_8, VALUE_8);

    U16 ADDRESS_16 = 0x1000;
    U32 VALUE_16 = M68K_READ_16(ADDRESS_16);

    printf("M68K READ 16 VALUE FROM ADDRESS 0x%04X: 0x%04X\n", ADDRESS_16, VALUE_16);

    U32 ADDRESS_32 = 0xADFF900;
    U32 VALUE_32 = M68K_READ_32(ADDRESS_32);

    printf("M68K READ 32 VALUE FROM ADDRESS 0x%08X: 0x%08X\n", ADDRESS_32, VALUE_32);

    return 0;
}
