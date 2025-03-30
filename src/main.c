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
    printf("U8 VALUE:%u\n", M68K_READ_8(VALUE));

    U16 VALUE_16 = 0xFF;
    printf("U16 VALUE:%u\n", M68K_READ_16(VALUE_16));

    U32 VALUE_32 = 0x100000;
    VALUE_32 = (M68K_READ_16(VALUE_16) << 16) | M68K_READ_16(VALUE_16);
    printf("U32 VALUE: %u\n", VALUE_32);
    
    U8 VALUE_8 = 0xAB;
    U8 ADDRESS_8 = 0x10;

    M68K_WRITE_8(ADDRESS_8, VALUE_8);
    U8 READ_8 = M68K_READ_8(ADDRESS_8);
    printf("U8: WROTE 0x%02X, READ 0x%02X\n", 
           VALUE_8, READ_8);

    return 0;
    
}
