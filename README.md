# lib68k
Motorola 680x0 Standalone Emulation Library

![image](https://github.com/user-attachments/assets/33aff4c6-3dce-4228-8aae-7ae8bcb37899)

# Motive:

The motive behind this repository is to provide a surrogate means of aiding in [relevant](https://github.com/hazzaclark/mdemu) Motorola 68000 developments.

Created using ANSI C99 standard, this project aims to promote an ease of use means of creating a library that emulates the likeness of these microprocessors

## Features:

 - Accurate and detailed CPU Emulation (statically typed, not interpretted)
 - Verbose Execution Model (precomputed Jump Table using Function Pointer)
 - Flexible Memory Mapper Scheme (RAM, ROM, IO and Vectors)
 - auto-disable for options
 - Specialised Interrupt Handler and Vector Exception Table  
 - Fully expandable Macros definitions (for pre-processing functions at compile time)

## Usage:

```
git clone this repository

make clean

make
```

the following output will vary depending on system:

### Windows:

```
lib68k.a
```

### UNIX:

```
lib68k.so
```

the makefile in question is universal, allowing for ease of use compiling on other systems

## Running the samples (w/ simulator)

In my experience, I have found that compiling with [clownassembler](https://github.com/Clownacy/clownassembler) works the best - for it's ease of use means of being able to provide input and output as well as providing in depth and concise warnings and error messages.

Follow the repository's instructions on how to work with that, or alternatively, use a 68K Assembler of your choosing

Another example is showcasing the Memory Read and Writes in accordance with the design of the struct which encompasses said logic

### Motorola 68K-specific Big Endian Reads, accounting for Endianness Swap

![image](https://github.com/user-attachments/assets/8f638290-1495-4cac-84ef-66730544cce8)

### Running the simulator with imm.bin

![image](https://github.com/user-attachments/assets/43c38c0a-4d10-4f39-833b-e5cd5e430c25)

## Auto-disable functionality:

For the sake of simplicity when it comes to the debugging utilities, I have implemented the functionality to turn HOOKS on and off

Simply use the oscillating macros of ``M68K_OPT_ON`` or ``M68K_OPT_OFF`` should you ever need more or less debugging information

```C
 	#define 	M68K_JUMP_HOOK 		M68K_OPT_ON
	#define		M68K_RTS_HOOK		M68K_OPT_ON
	#define		M68K_RESET_HOOK		M68K_OPT_ON

	 // TURN THE EQUALISER TO ON AND OFF

	#if M68K_JUMP_HOOK == M68K_OPT_ON
    #define M68K_BASE_JUMP_HOOK(ADDR, FROM_ADDR) \
        do { \
            printf("[JUMP TRACE] TO: 0x%08X FROM: 0x%08X\n", (ADDR), (FROM_ADDR)); \
        } while(0)
	#endif

	#if M68K_RTS_HOOK == M68K_OPT_ON
    #define M68K_BASE_RTS_HOOK(FROM_ADDR) \
        do { \
            printf("[RTS] 0x%04X\n", (FROM_ADDR)); \
        } while(0)
	#else
    	#define M68K_BASE_RTS_HOOK(FROM_ADDR) ((void)0)
	#endif

	#if M68K_RESET_HOOK == M68K_OPT_ON
    #define M68K_BASE_RES_HOOK(T0, T1, PC, SP) \
        do { \
            printf("RETURNED WITH TRACE LEVEL (T0: %d, T1: %d) -> CURRENT PC: %d -> CURRENT SP: 0x%04X\n", \
                  (T0), (T1), (PC), (SP)); \
        } while(0)
#else
    #define M68K_BASE_RES_HOOK(T0, T1, PC, SP) ((void)0)
#endif
```

## Execution Flow and reading Instructions in Memory

One of the intrinsic features encompassing lib68k is the ability to precompute the Opcodes at compile-time. 

This is achieved through the ``M68K_BUILD_OPCODE_TABLE`` function, which generates a static array of all of the presupposed elements within the table. 

From there, the pointer encompassing the generation of the Opcode Handler Table itself is linked to a function pointer.

The function pointer in question doesn't account for an end-all-be-all design approach in accessing Opcodes. Since everything is mapped by the struct irrespective of the context, at runtime it will mould to suit whichever circumstance is required of the Opcode being fetched in execution, (size, type, EA mode, IMM value, etc).

```c
OSTRUCT = M68K_OPCODE_HANDLER_TABLE;
while (OSTRUCT->HANDLER != NULL)
{
        #if USE_OPCODE_DEBUG == M68K_OPT_ON

        printf("PROCESSING OPCODE: MASK = 0x%04X, MATCH = 0x%04X, HANDLER = %p\n",
               OSTRUCT->MASK, OSTRUCT->MATCH, (void*)&OSTRUCT->HANDLER);

        #endif

        for (INDEX = 0; INDEX < 0x10000; INDEX++)
        {
            // IF THE CORRESPONDING OPCODE MASK FROM THE TABLE 
            // MATCHES HOW IT APPEARS IN TRAD 68K, USE THE CORRESPONDING AMOUNT OF CYCLES

            if ((INDEX & OSTRUCT->MASK) == OSTRUCT->MATCH)
            {
                M68K_OPCODE_JUMP_TABLE[INDEX] = OSTRUCT->HANDLER;
                CYCLE_RANGE[INDEX] = OSTRUCT->CYCLES;               
            }
        }

        OSTRUCT++;
    }
```

```c
// 68K.c - FROM M68K_EXEC

// READ THE CURRENTLY AVAILABLE INSTRUCTION INTO THE INDEX REGISTER
M68K_REG_IR = M68K_READ_MEMORY_16(M68K_REG_PC);

// MAP THAT NEWLY GATHERED OPCODE FROM THE IR INTO THE FUNCTION POINTER
M68K_OPCODE_JUMP_TABLE[M68K_REG_IR]();

// MANUALLY ADVANCE THE PC TO PROPERLY READ THE NEXT (PC advances are bound to change based on EA mode in Opcode def)
M68K_REG_PC += 2;
``` 

 # Sources:

[Motorola 680x0 Programmer Manual](https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf)

[Motorola 680x0 User Manual](https://www.nxp.com/docs/en/reference-manual/MC68000UM.pdf)

[68000 Memory Map](https://www.mwftr.com/ucF08/LEC05-68K-1.pdf)

[68K General-purpose manual](https://www.cpcwiki.eu/imgs/7/7a/MC68000_User%27s_Manual.pdf)

[68000 ISA](http://wpage.unina.it/rcanonic/didattica/ce1/docs/68000.pdf)

[lib68k_mem](https://github.com/hazzaclark/lib68k_mem)

[68K Instruction Timings](https://wiki.neogeodev.org/index.php?title=68k_instructions_timings)

[imHex (for being a brilliant Hex Editor)](https://github.com/WerWolv/ImHex)

[More 68K Instruction Timings](https://oldwww.nvg.ntnu.no/amiga/MC680x0_Sections/mc68000timing.HTML)
