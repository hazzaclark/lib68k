## lib68k - Opcode Handler Table structure

### How does it all work?

One of the many enigmatic design choices I made when creating my emulator was determining how I could create and define Opcode definitions.

I opted to create a statically-typed infrastructure which includes all of the Opcode definitions verbatim as they appear within the documentation provided (all down to their unique characteristics).
During initialisation, a static structure is declared which contains a mask and respective match value. Which presuppose the mask of the Opcode itself, and a match value which determines which Bits that Opcode encompasses at runtime.

This approaches comes in handy especially when dealing with variance in an Opcode family (for example, ADD and MOVE; which have varying addressing modes encoded in lower bit ranges).

From there, the table will iterate through each possible Opcode value against the mask/mach pattern in conjunction with the max 16 bit value.

Finally, the encompassing Opcode Handler Table gets evaluated into a Function Pointer which makes evaluation as efficient as possible due to being able to properly evaluate the Opcode Handler name against it's designated type.
```
┌─────────────────────────────────────────────────────────────────────────────┐
│                       LIB68K OPCODE HANDLER SCHEME                          │
└─────────────────────────────────────────────────────────────────────────────┘

STEP 1: STATIC HANDLER TABLE
┌────────────────────────────────────────────────────────────────────┐
│ OPCODE_HANDLER M68K_OPCODE_HANDLER_TABLE[] = {                     │
│ ┌──────────┬────────┬────────┬────────┐                            │
│ │ HANDLER  │  MASK  │ MATCH  │ CYCLES │                            │
│ ├──────────┼────────┼────────┼────────┤                            │
│ │HANDLER_A │ 0xFFF8 │ 0x4200 │   4    │                            │
│ │HANDLER_B │ 0xF1F8 │ 0xD000 │   8    │                            │
│ │HANDLER_C │ 0xFFFF │ 0x4E75 │  16    │                            │
│ │HANDLER_D │ 0xFFFF │ 0x4E71 │   4    │                            │
│ │  NULL    │  ---   │  ---   │  ---   │                            │
│ └──────────┴────────┴────────┴────────┘                            │
└────────────────────────────────────────────────────────────────────┘


STEP 2: ITERATION
┌────────────────────────────────────────────────────────────────────┐
│         // FUNCTION POINTER                                        │
|         void(*M68K_OPCODE_JUMP_TABLE[0x10000])(void);              │
│                                                                    │ 
|        // 0x10000 IS THE MAX 16 BIT VALUE                          │ 
│        for(INDEX = 0; INDEX < 0x10000; INDEX++                     │ 
|            // ASSUME WE HAVE A MATCH                               │   
|            M68K_OPCODE_JUMP_TABLE[INDEX] = OPCODE->HANDLER;        │
|                                                                    │  
|        // ITERATE CONTINOUSLY                                      │
│        OPCODE++;                                                   │ 
└────────────────────────────────────────────────────────────────────┘

STEP 3: READ FUNCTION POINTER INTO INDEX REGISTER

// READS CURRENT OPCODE BASED ON PC POSITION
M68K_REG_IR = M68K_READ_16(M68K_REG_PC);

// PASS THE IR INTO FP = RESULTS
M68K_OPCODE_JUMP_TABLE[M68K_REG_IR]();
```
