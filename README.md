# lib68k
Motorola 680x0 Standalone Emulation Library

![image](https://github.com/user-attachments/assets/9ad47554-e6a1-4380-9e8b-47e53ebf5483)

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

# Examples:

Below is an example provided to showcase how the ``OPCODE_HANDLER_TABLE`` is able to store and evaluate respective Opcodes and Operands

You can find the provided example Assembly code [here](https://github.com/hazzaclark/lib68k/blob/main/hello.asm)

![image](https://github.com/user-attachments/assets/b5b0dda9-a185-433c-9a20-b89427d756af)

![image](https://github.com/user-attachments/assets/efed0817-24ef-493f-87af-03bd1d0e4eba)

Another example is showcasing the Memory Read and Writes in accordance with the design of the struct which encompasses said logic

![image](https://github.com/user-attachments/assets/57698db6-5c48-4691-80e2-768a1b89cb0b)



# NOTE:

Some of the Opcodes and their extended addressing capabilities such as IMM, EA, DI, PI, etc - are all reserved for an eventual expansion on this Library.

As of right now, I wanted to focus on getting everything working with 68000 emulation to prioritise [mdemu](https://github.com/hazzaclark/mdemu/blob/main) developments

 # Sources:

[Motorola 680x0 Programmer Manual](https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf)

[Motorola 680x0 User Manual](https://www.nxp.com/docs/en/reference-manual/MC68000UM.pdf)

[68000 Memory Map](https://www.mwftr.com/ucF08/LEC05-68K-1.pdf)

[68000 ISA](http://wpage.unina.it/rcanonic/didattica/ce1/docs/68000.pdf)
