/* COPYRIHGT (C) HARRY CLARK 2025 */
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE EMULATOR'S OVERARCHING STDLIB FUNCTIONALITY */
/* AKIN TO THE LIKENESS OF GNU LIBC */

#ifndef M68K_STD_LIB
#define M68K_STD_LIB

/* NESTED INCLUDES */

#include "68K.h"
#include "common.h"

#if defined(USE_STD_LIB)
    #define USE_STD_LIB
#else
    #define USE_STD_LIB

    #define M68K_ERROR_EXIT(MESSAGE)                           \
        fprintf(stderr, "Error: " MESSAGE "\n");     \
        exit(1);                                                    \


    #define M68K_PERROR_EXIT(MESSAGE)                              \
        perror("Error: " MESSAGE "\n");                  \
        exit(1);                                                        \

    // GET THE CORRESPONDING AMOUNT OF LINE PERTAINS FILE
    // DISCARDING ANY AND ALL EOF CHARACTERS AND WEIRD TAB SPACES

    int GET_LINE(char* BUFFER, int CHARS, FILE* FILE)
    {
        int LEN = 0;

        if(fgets(BUFFER, CHARS, FILE) == NULL) return EXIT_FAILURE;

        LEN = strlen(BUFFER);

        if(LEN && (BUFFER[LEN - 1] == '\r'))
        {
            LEN--;
            BUFFER[LEN] = 0;
            return LEN;
        }

        return 0;
    }

    // VERY VERY SIMPLE HEURISTIC

    int IS_DIR(const char* TOKEN) 
    {
        if (TOKEN) 
        {
            if (strchr(TOKEN, '.') != NULL || 
                strncmp(TOKEN, "ORG", 3) == 0 || 
                strncmp(TOKEN, "EQU", 3) == 0 || 
                strncmp(TOKEN, "END", 3) == 0) 
                { return 1; }
        }

        return 0;
    }

void PROCESS_LINE(char* LINE_BUFFER, int LINE_INDEX) 
{
    char OPCODES[MAX_OPCODE_INPUT];
    char OPERANDS[MAX_OPERAND_INPUT];
    char LABEL[MAX_LINE_BUFFER];
    char* LINE_START;

    memset(OPCODES, 0, sizeof(OPCODES));
    memset(OPERANDS, 0, sizeof(OPERANDS));
    memset(LABEL, 0, sizeof(LABEL));

    char* COMMENT = strstr(LINE_BUFFER, ";");
    if (COMMENT) *COMMENT = '\0';

    LINE_START = LINE_BUFFER;
    while (*LINE_START && isspace((unsigned char)*LINE_START)) LINE_START++;

    if (*LINE_START == '\0') 
    {
        return; 
    }

    EXTRACT_LABEL(&LINE_START, LABEL);
    EXTRACT_OPCODE_AND_OPERANDS(LINE_START, OPCODES, OPERANDS);

    if (IS_DIR(OPCODES)) 
    {
        HANDLE_DIRECTIVE(LINE_INDEX, LABEL, OPCODES, OPERANDS);
    }
    else 
    {
        HANDLE_OPCODE(LINE_INDEX, LABEL, OPCODES, OPERANDS);
    }
}

void EXTRACT_LABEL(char** LINE_START, char* LABEL) 
{
    char* LABEL_END = strchr(*LINE_START, ':');
    if (LABEL_END) 
    {
        strncpy(LABEL, *LINE_START, LABEL_END - *LINE_START);
        LABEL[LABEL_END - *LINE_START] = '\0';
        *LINE_START = LABEL_END + 1;

        while (**LINE_START && isspace((unsigned char)**LINE_START)) (*LINE_START)++;
    }
}

void EXTRACT_OPCODE_AND_OPERANDS(char* LINE_START, char* OPCODES, char* OPERANDS) 
{
    char* TOKEN = strtok(LINE_START, " \t");
    if (TOKEN) 
    {
        strncpy(OPCODES, TOKEN, MAX_OPCODE_INPUT - 1);

        char* OPERANDS_START = strtok(NULL, "\n");
        if (OPERANDS_START) 
        {
            while (*OPERANDS_START && isspace((unsigned char)*OPERANDS_START)) OPERANDS_START++;
            strncpy(OPERANDS, OPERANDS_START, MAX_OPERAND_INPUT - 1);
        }
    }
}

void HANDLE_DIRECTIVE(int LINE_INDEX, char* LABEL, char* OPCODES, char* OPERANDS) 
{
    if (*LABEL) 
    {
        printf("Line %d: Label: %s with Directive %s and Operands: %s\n", LINE_INDEX + 1, LABEL, OPCODES, OPERANDS);
    } 
    else 
    {
        printf("Line %d: Directive %s with Operands: %s\n", LINE_INDEX + 1, OPCODES, OPERANDS);
    }
}

void HANDLE_OPCODE(int LINE_INDEX, char* LABEL, char* OPCODES, char* OPERANDS) 
{
    if (*LABEL) 
    {
        printf("Line %d: Label: %s with Opcode %s and Operands: %s\n", LINE_INDEX + 1, LABEL, OPCODES, OPERANDS);
    } 
    else 
    {
        printf("Line %d: Opcode %s with Operands: %s\n", LINE_INDEX + 1, OPCODES, OPERANDS);
    }
}


#endif
#endif
