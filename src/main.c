/* COPYRIHGT (C) HARRY CLARK 2024 */
/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "util.h"
#include "68K.h"
#include "68KOPCODE.h"

static LIB_BASE* LIB68K;
static OPCODE* OPCODE_BASE;

#define         PC_MAX_VALUE            0x10000

/* THE MAIN ENCOMPASSING FUNCTION TO PARSE THE CONTENTS PROVIDED THROUGH THE ARGS */
/* READ THE CONTENTS, GET THE MAX SIZE OF EACH LINE, SKIP WHITESPACE AND VALIDATE EACH OPCODE */
/* IN THE OPCODE TABLE */

void PROC_FILE(char* FILENAME)
{
    char LINE_BUFFER[MAX_LINE_BUFFER];
    char OPCODES[MAX_OPCODE_INPUT];
    char OPERANDS[MAX_LINE_BUFFER] = "";
    char* WHITESPACE = strtok(LINE_BUFFER, "\t\n");

    int LINE_INDEX = 0;

    INPUT = fopen(FILENAME, "r");

    if(!INPUT)
    {
        perror("Failed the open source file\n");
        exit(EXIT_FAILURE);
    }

    // INDEX THROUGH EVERY AVAILABLE LINE

    while(fgets(LINE_BUFFER, sizeof(LINE_INDEX), INPUT) == 0)
    {
        LINE_INDEX++;
        
        // PERFORM WHITESPACE CHECKS

        if(WHITESPACE[0] == '\0' || WHITESPACE[0] == ';') { continue; }

    }

    printf("Opcode: %s\n", OPCODES);
    printf("Operands: %s\n", OPERANDS);

    // DETERMINE THE VALIDITY OF THE OPCODES PROVIDED IN RELATION TO WHAT IS AVAILABLE
    // WITHIN THE TABLE - WHICH IS A CONJUNCTIVE EFFORT BETWEEN 68KOPCODE.c AS WELL AS
    // THE HANDLER HELPER FUNCTION

    OPCODE_BASE = FIND_OPCODE(OPCODES, 16);

    if(!OPCODE_BASE)
    {
        fprintf(stderr, "Couldn't find valid Opcode at Line %d: %s\n", LINE_INDEX, (char*)OPCODE_BASE);
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char** argv)
{
    printf("====================================================\n");
    printf("HARRY CLARK - MOTOROLA 680x0 EMULATOR\n");
    printf("====================================================\n");
    
    if(argc < 1)
    {
        char* INDEX;
        strncpy(LIB68K->OUTPUT_PATH, argv[1], sizeof(LIB68K->OUTPUT_PATH));

        // RETURNS A NEW OCCRUANCE OF A NEW LINE CHARACTER TO DENOTE THE OUTPUT PATH

        for(INDEX = strchr(LIB68K->OUTPUT_PATH, '\\'); INDEX; INDEX = strchr(INDEX, '\\'))
        {
            *INDEX = '/';
        }

        // EVALUATE THE LENGTH OF THE NEW OUTPUT PATH OF THE FILE INPUT

        if(LIB68K->OUTPUT_PATH[strlen(LIB68K->OUTPUT_PATH) - 1 ] != '/') 
        {
            strcat(LIB68K->OUTPUT_PATH, "/");
        }
    }

    if(argc > 2) { strcpy(LIB68K->INPUT_FILE, argv[2]); }

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


    return 0; 
}
