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

![image](https://github.com/user-attachments/assets/e909fa4b-4a60-4f26-a969-4fa499449c71)

 # Sources:

[Motorola 680x0 Programmer Manual](https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf)

[Motorola 680x0 User Manual](https://www.nxp.com/docs/en/reference-manual/MC68000UM.pdf)

[68000 Memory Map](https://www.mwftr.com/ucF08/LEC05-68K-1.pdf)

[68000 ISA](http://wpage.unina.it/rcanonic/didattica/ce1/docs/68000.pdf)
