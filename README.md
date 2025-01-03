# lib68k
Motorola 680x0 Standalone Emulation Library

![image](https://github.com/user-attachments/assets/19208caf-24ed-4018-82dc-72e72ceb64e9)

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

# Example:

Below is an example provided to showcase how the ``OPCODE_HANDLER_TABLE`` is able to store and evaluate respective Opcodes and Operands

You can find the provided example Assembly code [here](https://github.com/hazzaclark/lib68k/blob/main/hello.asm)

![image](https://github.com/user-attachments/assets/d425f711-eaed-4545-bc08-4b3917d41955)


 # Sources:

[Motorola 680x0 Programmer Manual](https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf)

[Motorola 680x0 User Manual](https://www.nxp.com/docs/en/reference-manual/MC68000UM.pdf)

[68000 Memory Map](https://www.mwftr.com/ucF08/LEC05-68K-1.pdf)

[68000 ISA](http://wpage.unina.it/rcanonic/didattica/ce1/docs/68000.pdf)
