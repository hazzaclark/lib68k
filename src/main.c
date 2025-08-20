/* COPYRIGHT (C) HARRY CLARK 2025 */
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* NESTED INCLUDES */

#include "68K.h"
#include "util.h"

#undef USE_68K
#undef USE_CONFIG
#undef USE_OPCODE
#undef USE_MEM

int main(int argc, char** argv) 
{
    printf("===================================================\n");
    printf("      HARRY CLARK - MOTOROLA 680x0 EMULATOR        \n");
    printf("===================================================\n");

    if(argc < 2)
    {
        printf("USAGE: %s <samples/FILE.bin>\n", argv[0]);
        return 1;
    }
    
    ENABLED_FLAGS = M68K_OPT_FLAGS;
    SET_TRACE_FLAGS(1, 0);
    SHOW_TRACE_STATUS();
    
    M68K_INIT();
    SHOW_MEMORY_MAPS();

    int FILE_SIZE = LOAD_BINARY_FILE(argv[1], M68K_REG_PC);

    M68K_PULSE_RESET();

    printf("SUCCESSFULLY LOADED -> %s WITH SIZE: %d%s AT PC VALUE -> 0x%04X\n\n", argv[1], 
        FORMAT_SIZE(FILE_SIZE), FORMAT_UNIT(FILE_SIZE), M68K_REG_PC);

    // ASSIGN THE CURRENT LOAD ADDR TO PC
    // THE PC WILL AUTO INCREMENT BASED ON INSTRUCTIONS

    M68K_EXEC(0);

    SHOW_MEMORY_MAPS();
    return 0;
}
