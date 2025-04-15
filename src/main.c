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

    U32 LOAD_ADDR = 0x00100000;
    if(argc > 2)
    {
        LOAD_ADDR = strtoul(argv[2], NULL, 16);
    }

    SHOW_TRACE_STATUS();
    ENABLE_TRACE_FLAG(M68K_OPT_BASIC);
    
    M68K_INIT();
    M68K_SET_CPU_TYPE(M68K_CPU_000);

    printf("CPU SET TO TYPE: %d\n", M68K_CPU_000);
    printf("LOADING BINARY FILE: %s AT 0x%08X\n", argv[1], LOAD_ADDR);

    int FILE_SIZE = LOAD_BINARY_FILE(argv[1], LOAD_ADDR);
    if(FILE_SIZE < 0)
    {
        printf("FAILED TO LOAD BINARY FILE\n");
        return 1;
    }

    printf("SUCCESSFULLY LOADED %d BYTES\n", FILE_SIZE);

    // ASSIGN THE CURRENT LOAD ADDR TO PC
    // THE PC WILL AUTO INCREMENT BASED ON INSTRUCTIONS
    M68K_REG_PC = LOAD_ADDR;

    printf("BEGINNING EXECUTION AT 0x%08X\n", LOAD_ADDR);
    M68K_EXEC(500);

    return 0;
}
