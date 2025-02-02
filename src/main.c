/* COPYRIHGT (C) HARRY CLARK 2025*/
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "util.h"
#include "68K.h"
#include "68KOPCODE.h"
#include "68KSTD.h"

static LIB_BASE* LIB68K;

#define         PC_MAX_VALUE            0x10000

/* THE MAIN ENCOMPASSING FUNCTION TO PARSE THE CONTENTS PROVIDED THROUGH THE ARGS */
/* READ THE CONTENTS, GET THE MAX SIZE OF EACH LINE, SKIP WHITESPACE AND VALIDATE EACH OPCODE */
/* IN THE OPCODE TABLE */

void PROCESS_FILE(char* FILENAME) 
{
    char LINE_BUFFER[MAX_LINE_BUFFER];
    FILE* INPUT;
    int LINE_INDEX = 0;

    INPUT = fopen(FILENAME, "r");
    if (!INPUT) 
    {
        perror("Failed to open source file\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(LINE_BUFFER, sizeof(LINE_BUFFER), INPUT)) 
    {
        PROCESS_LINE(LINE_BUFFER, LINE_INDEX);
        LINE_INDEX++;
    }

    fclose(INPUT);
}

int main(int argc, char** argv)
{
    printf("====================================================\n");
    printf("HARRY CLARK - MOTOROLA 680x0 EMULATOR\n");
    printf("====================================================\n");
    
    if (argc < 2) 
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

    if (argc > 2) 
    {
        char* INDEX;
        strncpy(LIB68K->OUTPUT_PATH, argv[2], sizeof(LIB68K->OUTPUT_PATH));

        for(INDEX = strchr(LIB68K->OUTPUT_PATH, '\\'); INDEX; INDEX = strchr(INDEX, '\\')) 
        {
            *INDEX = '/';
        }

        if(LIB68K->OUTPUT_PATH[strlen(LIB68K->OUTPUT_PATH) - 1] != '/') 
        {
            strcat(LIB68K->OUTPUT_PATH, "/");
        }
    }

    strcpy(LIB68K->INPUT_FILE, argv[1]);

    printf("Initialising 68000\n");
    M68K_INIT();
    M68K_EXEC();
    printf("68000 is running: %p\n", (void*)&CPU);

    printf("Setting 68K Program Counter\n");
    M68K_SET_REGISTERS(M68K_REG_PC, PC_MAX_VALUE);   
    printf("68K Program Counter defined with Value: %d\n", PC_MAX_VALUE);

    printf("====================================================\n");

    printf("Setting 68K Stack Pointer\n");
    M68K_GET_REGISTERS(&CPU, M68K_REG_D[M68K_REG_SP]);
    printf("68K Stack Pointer defined with Value: %d\n", (int)M68K_REG_SP);

    printf("\nProcessing Source File: %s\n", LIB68K->INPUT_FILE);
    PROCESS_FILE(LIB68K->INPUT_FILE);

    free(LIB68K);
    return 0;
}
