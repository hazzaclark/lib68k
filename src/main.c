/* COPYRIGHT (C) HARRY CLARK 2025 */
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* NESTED INCLUDES */

#include "util.h"
#include "68K.h"
#include "68KCONF.h"
#include "68KOPCODE.h"
#include "68KSTD.h"
#include "68KMEM.h"

int main(int argc, char** argv) 
{
    printf("====================================================\n");
    printf("HARRY CLARK - MOTOROLA 680x0 EMULATOR\n");
    printf("====================================================\n");

    if(argc < 2)
    {
        printf("USAGE: %s <FILE>\n", argv[0]);
        return 1;
    }


    // ENABLE THE CURRENT VERBOSE TRACE INFORMATION
    #if M68K_USE_TRACE_CONFIG == M68K_OPT_OFF

    ENABLED_FLAGS = M68K_OPT_FLAGS;
    SET_TRACE_FLAGS(1, 0);
    SHOW_TRACE_STATUS();

    #endif
    
    M68K_INIT();
    printf("LOADING BINARY FILE: %s AT 0x%04X\n", argv[1], M68K_REG_PC);

    int FILE_SIZE = LOAD_BINARY_FILE(argv[1], M68K_REG_PC);
    if(FILE_SIZE < 0)
    {
        printf("FAILED TO LOAD BINARY FILE\n");
        return 1;
    }

    printf("SUCCESSFULLY LOADED %d BYTES\n", FILE_SIZE);

    // ASSIGN THE CURRENT LOAD ADDR TO PC
    // THE PC WILL AUTO INCREMENT BASED ON INSTRUCTIONS

    printf("BEGINNING EXECUTION AT PC: 0x%04X\n", M68K_REG_PC);
    M68K_EXEC(500);

    return 0;
}
