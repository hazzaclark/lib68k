/* COPYRIHGT (C) HARRY CLARK 2024 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE PROGRAM */

/* NESTED INCLUDES */

#include "util.h"
#include "68K.h"
#include "68KOPCODE.h"

static LIB_BASE* LIB68K;

int main(int argc, char** argv)
{
    printf("HARRY CLARK - MOTOROLA 680x0 EMULATOR\n");
    
    if(argc > 1)
    {
        char* INDEX;
        strncpy(LIB68K->OUTPUT_PATH, argv[1], sizeof(LIB68K->OUTPUT_PATH));

        // RETURNS A NEW OCCRUANCE OF A NEW LINE CHARACTER TO DENOTE THE OUTPUT PATH

        for(INDEX = strchr(LIB68K->OUTPUT_PATH, '\\'); INDEX; INDEX = strchr(INDEX, '\\'))
        {
            *INDEX = '/';
        }
    }    
}
