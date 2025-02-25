/* COPYRIHGT (C) HARRY CLARK 2025*/
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "util.h"
#include "68K.h"
#include "68KOPCODE.h"
#include "68KSTD.h"

static LIB_BASE* LIB68K;

#define     PC_MAX_VALUE    0x1000  
#define     SP_INIT_VALUE   0xFFFFF  

void LOAD_SRC_FILE(const char* FILENAME, uint32_t LOAD_ADDRESS) 
{
    FILE* INPUT = fopen(FILENAME, "rb");
    if (!INPUT) 
    {
        perror("Failed to open binary file\n");
        exit(EXIT_FAILURE);
    }

    fseek(INPUT, 0, SEEK_END);
    long FILE_SIZE = ftell(INPUT);
    fseek(INPUT, 0, SEEK_SET);

    printf("File size: %ld bytes\n", FILE_SIZE);  

    if ((unsigned long)FILE_SIZE > MAX_MEMORY_SIZE - LOAD_ADDRESS) 
    {
        fprintf(stderr, "File too large to fit in memory\n");
        fclose(INPUT);
        exit(EXIT_FAILURE);
    }

    uint8_t BUFFER;
    uint32_t ADDRESS = LOAD_ADDRESS;
    size_t BYTES_LOADED = 0;

    while (1) 
    {
        UNK ITEMS_READ = fread(&BUFFER, 1, 1, INPUT);
        if (ITEMS_READ != 1) { break; }

        printf("Read byte: 0x%02X\n", BUFFER); 
        M68K_WRITE_8(ADDRESS, BUFFER);

        ADDRESS++;
        BYTES_LOADED++;
    }

    fclose(INPUT);
    printf("Loaded %zu bytes into memory at address 0x%04X\n", BYTES_LOADED, LOAD_ADDRESS);
}

int main(int argc, char** argv) 
{
    printf("====================================================\n");
    printf("HARRY CLARK - MOTOROLA 680x0 EMULATOR\n");
    printf("====================================================\n");

    if (argc != 2) 
    {
        printf("Usage: %s INPUT_FILE \n", argv[0]);
        return 1;
    }

    LIB68K = (LIB_BASE*)malloc(sizeof(LIB_BASE));
    if (!LIB68K) 
    {
        perror("Failed to allocate LIB68K");
        return 1;
    }

    strcpy(LIB68K->INPUT_FILE, argv[1]);

    printf("Initialising 68000\n");

    M68K_INIT();

    uint32_t LOAD_ADDRESS = 0x0000; 
    LOAD_SRC_FILE(LIB68K->INPUT_FILE, LOAD_ADDRESS);

    /* SET THE PROGRAM COUNTER AND STACK POINTER */
    printf("Setting 68K Program Counter\n");
    M68K_SET_REGISTERS(M68K_REG_PC, LOAD_ADDRESS);
    printf("68K Program Counter defined with Value: %04X\n", LOAD_ADDRESS);

    printf("====================================================\n");

    printf("Setting 68K Stack Pointer\n");
    M68K_SET_REGISTERS(M68K_REG_SP, SP_INIT_VALUE);
    printf("68K Stack Pointer defined with Value: %04X\n", SP_INIT_VALUE);

    printf("====================================================\n");

    printf("Executing Simulator...\n");

    int CYCLE_COUNT = 0;
    const int MAX_CYCLES = 100000;

    while (CYCLE_COUNT < MAX_CYCLES && !M68K_CPU_STOPPED) 
    {
        printf("CURRENT EXEC\n");

        int CYCLES_EXECUTED = M68K_EXEC(100);

        CYCLE_COUNT += CYCLES_EXECUTED;
        printf("CYCLES EXECUTED: %d, TOTAL CYCLES: %d\n", CYCLES_EXECUTED, CYCLE_COUNT);

        U32 PC_VALUE = M68K_GET_REGISTERS(NULL, M68K_REG_PC);
        if (PC_VALUE == MAX_MEMORY_SIZE) { break; }
    }

    free(LIB68K);
    return 0;
}
