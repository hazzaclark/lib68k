/* COPYRIHGT (C) HARRY CLARK 2025*/
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "util.h"
#include "68K.h"
#include "68KCONF.h"
#include "68KOPCODE.h"
#include "68KSTD.h"

int main(void) 
{
    printf("====================================================\n");
    printf("HARRY CLARK - MOTOROLA 680x0 EMULATOR\n");
    printf("====================================================\n");
    
    M68K_INIT();
    M68K_EXEC(100);

    U8 VALUE = 0xF;
    printf("U8 VALUE:0x%02X, DEC: %d\n", M68K_READ_8(VALUE), VALUE);

    U16 VALUE_16 = 0xFFFF;
    printf("U16 VALUE:0x%04X, DEC: %d\n", M68K_READ_16(VALUE_16), VALUE_16);

    U32 ADDRESS_32 = 0x1000;
    U32 VALUE_32 = 0x12345678;
    M68K_WRITE_32(ADDRESS_32, VALUE_32);
    printf("U32 VALUE:0x%08X, DEC: %d\n", M68K_READ_32(ADDRESS_32), VALUE_32);
    
    U8 VALUE_8 = 0xFF;
    U8 ADDRESS_8 = 0x10;

    M68K_WRITE_8(ADDRESS_8, VALUE_8);
    U8 READ_8 = M68K_READ_8(ADDRESS_8);
    printf("U8: WROTE 0x%02X, READ 0x%02X\n", 
           VALUE_8, READ_8);

    return 0;
}
