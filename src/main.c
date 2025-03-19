/* COPYRIGHT (C) HARRY CLARK 2025 */
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "util.h"
#include "68K.h"
#include "68KOPCODE.h"
#include "68KSTD.h"
#include <stdio.h>
#include <stdlib.h>

/* FUNCTION DECLARATIONS */

void SIMULATOR(const char* FILE_PATH);
void PRINT_REGISTERS(void);

/* SIMULATOR FUNCTION: LOADS AND EXECUTES A BINARY FILE */

void SIMULATOR(const char* FILE_PATH)
{
    printf("====================================================\n");
    printf("SIMULATOR: LOADING AND EXECUTING BINARY FILE\n");
    printf("====================================================\n");

    /* INITIALISE THE CPU */
    M68K_INIT();

    /* OPEN THE BINARY FILE */
    FILE* FILE = fopen(FILE_PATH, "rb");
    if (!FILE)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    /* DETERMINE THE FILE SIZE */
    fseek(FILE, 0, SEEK_END);
    long FILE_SIZE = ftell(FILE);
    fseek(FILE, 0, SEEK_SET);

    /* ALLOCATE MEMORY FOR THE BINARY FILE */
    U8* MEMORY = (U8*)malloc(FILE_SIZE);
    if (!MEMORY)
    {
        perror("Failed to allocate memory");
        fclose(FILE);
        exit(EXIT_FAILURE);
    }

    /* READ THE BINARY FILE INTO MEMORY */
    fread(MEMORY, 1, FILE_SIZE, FILE);
    fclose(FILE);

    /* SET THE PROGRAM COUNTER TO THE START OF THE LOADED BINARY */
    M68K_SET_REGISTERS(M68K_REG_PC, 0x0000);

    /* PRINT INITIAL REGISTER VALUES */
    printf("INITIAL REGISTER VALUES:\n");
    PRINT_REGISTERS();

    printf("\nSIMULATING INSTRUCTIONS...\n");

    /* MAIN SIMULATION LOOP */
    while (M68K_GET_REGISTERS(&CPU, M68K_REG_PC) < (UNK)FILE_SIZE)
    {
        /* FETCH THE NEXT OPCODE */
        U16 OPCODE = (MEMORY[M68K_GET_REGISTERS(&CPU, M68K_REG_PC)] << 8) | 
                     MEMORY[M68K_GET_REGISTERS(&CPU, M68K_REG_PC) + 1];

        printf("\nEXECUTING OPCODE: 0x%04X at PC: 0x%08X\n", 
               OPCODE, M68K_GET_REGISTERS(&CPU, M68K_REG_PC));

        /* EXECUTE THE OPCODE IF IT EXISTS IN THE JUMP TABLE */
        if (M68K_OPCODE_JUMP_TABLE[OPCODE])
        {
            M68K_OPCODE_JUMP_TABLE[OPCODE]();
        }
        else
        {
            printf("UNKNOWN OPCODE: 0x%04X\n", OPCODE);
            break;
        }

        /* INCREMENT THE PROGRAM COUNTER */
        M68K_SET_REGISTERS(M68K_REG_PC, M68K_GET_REGISTERS(&CPU, M68K_REG_PC) + 2);
        PRINT_REGISTERS();
    }

    printf("\nFINAL REGISTER VALUES:\n");
    PRINT_REGISTERS();

    printf("\nSIMULATION COMPLETE.\n");
    free(MEMORY);
}


/* FUNCTION TO PRINT REGISTER VALUES */

void PRINT_REGISTERS(void)
{
    printf("----------------------------------------------------\n");
    printf("REGISTER VALUES:\n");
    
    for (int i = 0; i < 8; ++i)
    {
        printf("D%d: 0x%08X\n", i, M68K_GET_REGISTERS(&CPU, CPU.DATA_REGISTER[0] + i));
    }

    for (int i = 0; i < 8; ++i)
    {
        printf("A%d: 0x%08X\n", i, M68K_GET_REGISTERS(&CPU, CPU.ADDRESS_REGISTER[0] + i));
    }

    printf("PC: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_REG_PC));
    printf("SR: 0x%04X\n", M68K_GET_REGISTERS(&CPU, M68K_REG_SR));
    printf("----------------------------------------------------\n");
}

int main(int argc, char** argv) 
{
    if (argc != 2)
    {
        printf("Usage: %s <INPUT_FILE.bin>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("====================================================\n");
    printf("HARRY CLARK - MOTOROLA 680x0 EMULATOR\n");
    printf("====================================================\n");

    printf("Initialising 68000\n");

    M68K_INIT();

    SIMULATOR(argv[1]);

    return 0;
}
