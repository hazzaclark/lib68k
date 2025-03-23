/* COPYRIHGT (C) HARRY CLARK 2025*/
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "util.h"
#include "68K.h"
#include "68KCONF.h"
#include "68KOPCODE.h"
#include "68KSTD.h"

void CALLBACK(void)
{
    printf("CALLBACK CALLED");
}

int main(void) 
{
    printf("====================================================\n");
    printf("HARRY CLARK - MOTOROLA 680x0 EMULATOR\n");
    printf("====================================================\n");

    printf("Initialising 68000\n");

    M68K_INIT();

    U8 VALUE = 0xF;
    printf("U8 VALUE:%u\n", M68K_READ_8(VALUE));

    U16 VALUE_16 = 0xFF;
    printf("U16 VALUE:%u\n", M68K_READ_16(VALUE_16));

    U32 VALUE_32 = 0x10000;
    printf("U32 VALUE:%u\n", M68K_READ_32(VALUE_32));

    // TESTING FUNCTION CALLBACK TO BE ABLE TO PROPERLY ALLOCATE
    // AND DESIGNATE MEMORY IN SPECIFIC PREFETCH INSTANCES

    M68K_SET_FUNC_CALLBACK(CALLBACK);

    if(M68K_SET_FC_ACK != NULL)
    {
        M68K_SET_FC_ACK();
    }

    return 0;
}
