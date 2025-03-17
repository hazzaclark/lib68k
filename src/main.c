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

void SIMULATOR(const char* FILE_PATH);

void SIMULATOR(const char* FILE_PATH)
{
    printf("====================================================\n");
    printf("SIMULATOR: LOADING AND EXECUTING BINARY FILE\n");
    printf("====================================================\n");

    /* INITIALISE THE CPU */
    M68K_INIT();

    FILE* FILE = fopen(FILE_PATH, "rb");
    if (!FILE)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fseek(FILE, 0, SEEK_END);
    long FILE_SIZE = ftell(FILE);
    fseek(FILE, 0, SEEK_SET);

    U8* MEMORY = (U8*)malloc(FILE_SIZE);
    if (!MEMORY)
    {
        perror("Failed to allocate memory");
        fclose(FILE);
        exit(EXIT_FAILURE);
    }

    fread(MEMORY, 1, FILE_SIZE, FILE);
    fclose(FILE);

    /* SET THE PROGRAM COUNTER TO THE START OF THE LOADED BINARY */
    M68K_SET_REGISTERS(M68K_PC, 0x0000);

    printf("INITIAL REGISTER VALUES:\n");
    printf("D0: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_D0));
    printf("D1: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_D1));
    printf("A0: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_A0));
    printf("A1: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_A1));
    printf("PC: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_PC));

    printf("\nSIMULATING INSTRUCTIONS...\n");

    while (M68K_GET_REGISTERS(&CPU, M68K_PC) < (unsigned long)FILE_SIZE)
    {
        U16 OPCODE = (MEMORY[M68K_GET_REGISTERS(&CPU, M68K_PC)] << 8) | MEMORY[M68K_GET_REGISTERS(&CPU, M68K_PC) + 1];
        printf("EXECUTING OPCODE: 0x%04X at PC: 0x%08X\n", OPCODE, M68K_GET_REGISTERS(&CPU, M68K_PC));

        if (M68K_OPCODE_JUMP_TABLE[OPCODE])
        {
            M68K_OPCODE_JUMP_TABLE[OPCODE]();
        }
        else
        {
            printf("UNKNOWN OPCODE: 0x%04X\n", OPCODE);
            break;
        }

        M68K_SET_REGISTERS(M68K_PC, M68K_GET_REGISTERS(&CPU, M68K_PC) + 2);
    }

    printf("\nFINAL REGISTER VALUES:\n");
    printf("D0: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_D0));
    printf("D1: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_D1));
    printf("A0: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_A0));
    printf("A1: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_A1));
    printf("PC: 0x%08X\n", M68K_GET_REGISTERS(&CPU, M68K_PC));

    printf("\nSIMULATION COMPLETE.\n");
    free(MEMORY);
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
