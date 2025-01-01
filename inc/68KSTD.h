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

    #define M68K_ERROR_EXIT(MESSAGE, ...)                           \
        fprintf(stderr, "Error: " MESSAGE "\n", ##__VA_ARGS__);     \
        exit(1);                                                    \


    #define M68K_PERROR_EXIT(MESSAGE, ...)                              \
        perror("Error: " MESSAGE "\n", ##__VA_ARGS__);                  \
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


#endif
#endif
