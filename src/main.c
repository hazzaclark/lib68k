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

    if(argc < 3)
    {
        printf("USAGE: %s <FILE> [LOAD ADDR]\n", argv[0]);
        return 1;
    }

    // CONVERT THE LOAD ADDRESS INTO THE PROPER UNSIGNED MASK
    // WHICH IS REQUIRED TO BE ABLE TO BE READ BY THE MEMORY MAPPER

    if(argc > 2)
    {
        M68K_REG_PC = strtoul(argv[2], NULL, 16);
    }

    // ENABLE THE CURRENT VERBOSE TRACE INFORMATION
    // THIS CAN BE TURNED USING M68K_OPT_OFF IN 68KCONF.h AND 68KMEM.h

    ENABLED_FLAGS = M68K_OPT_FLAGS;
    SET_TRACE_FLAGS(1, 0);
    SHOW_TRACE_STATUS();
    
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
