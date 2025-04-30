# lib68k
Motorola 680x0 Standalone Emulation Library

# Motive:

The motive behind this repository is to provide a surrogate means of aiding in relevant Motorola 68000 developments.

Such is the case with one of the many ambitions, to create a dedicated OS Kernel specifically for this series of microprocessors

Created using ANSI C99 standard, this project aims to promote an ease of use means of creating a library that emulates the likeness of these microprocessors

# Usage:

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

### Motorola 68K-specific Big Endian Reads, handled with LE in mind

![image](https://github.com/user-attachments/assets/2bb2ef5e-c1c6-46c1-9c2c-928b9809aa50)

### Running the simulator with add.bin

![image](https://github.com/user-attachments/assets/49218a67-8ded-4036-84bd-1c0194ed4d89)


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

 # Sources:

[Motorola 680x0 Programmer Manual](https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf)

[Motorola 680x0 User Manual](https://www.nxp.com/docs/en/reference-manual/MC68000UM.pdf)

[68000 Memory Map](https://www.mwftr.com/ucF08/LEC05-68K-1.pdf)

[68K General-purpose manual](https://www.cpcwiki.eu/imgs/7/7a/MC68000_User%27s_Manual.pdf)

[68000 ISA](http://wpage.unina.it/rcanonic/didattica/ce1/docs/68000.pdf)

[lib68k_mem](https://github.com/hazzaclark/lib68k_mem)
