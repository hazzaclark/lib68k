/* COPYRIHGT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDSS THE MAIN FUNCTIONALITY SURROUNDING THE UTILITY */
/* OF THE CPU AND IT'S SUBSEQUENT DIRECTIVES AND DEFINES */

/* ANY AND ALL OF THE CORRESPONDING DEFINES AND DECLARATIONS CAN BE */
/* ATTRIBUTED TO THE INFROMATION FOUND HERE: https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf  */ 

/* NESTED INCLUDES */

#include "68K.h"
#include "68KCONF.h"
#include "common.h"
#include "util.h"

#undef USE_68K
#undef BUILD_OP_TABLE

unsigned char CYCLE_RANGE[0x10000];

/*==================================================================================*/
/*                          68000 MAIN CPU FUNCTIONALIY                             */
/*==================================================================================*/

/* ACCESS EACH RESPECTIVE REGISTER FROM THE ENUMERATION */
/* RETURN THE CORRESPONDENCE IN RELATION TO THE SIZE */

unsigned int M68K_GET_REGISTERS(struct CPU_68K* CPU, int REGISTER)
{
    switch (REGISTER)
    {
        case M68K_D0: return CPU->DATA_REGISTER[0];
        case M68K_D1: return CPU->DATA_REGISTER[1];
        case M68K_D2: return CPU->DATA_REGISTER[2];
        case M68K_D3: return CPU->DATA_REGISTER[3];
        case M68K_D4: return CPU->DATA_REGISTER[4];
        case M68K_D5: return CPU->DATA_REGISTER[5];
        case M68K_D6: return CPU->DATA_REGISTER[6];
        case M68K_D7: return CPU->DATA_REGISTER[7];
        case M68K_A0: return CPU->ADDRESS_REGISTER[0];
        case M68K_A1: return CPU->ADDRESS_REGISTER[1];
        case M68K_A2: return CPU->ADDRESS_REGISTER[2];
        case M68K_A3: return CPU->ADDRESS_REGISTER[3];
        case M68K_A4: return CPU->ADDRESS_REGISTER[4];
        case M68K_A5: return CPU->ADDRESS_REGISTER[5];
        case M68K_A6: return CPU->ADDRESS_REGISTER[6];
        case M68K_A7: return CPU->ADDRESS_REGISTER[7];
        case M68K_PC: return CPU->PC;
        case M68K_SR: return CPU->STATUS_REGISTER;
        case M68K_IR: return CPU->INDEX_REGISTER;
        case M68K_SP: return CPU->REGISTER_BASE[15];

        default: return 0;
    }
}

/* IN A SIMILAR VEIN TO THE FUNCTION ABOVE, THIS FUNCTION WILL FOCUS MOREOVER */
/* ON THE SETTING OF THESE VALUES */

void M68K_SET_REGISTERS(unsigned int REGISTER, unsigned int VALUE)
{
    switch (REGISTER)
    {
        case M68K_D0: M68K_REG_D[0] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_D1: M68K_REG_D[1] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_D2: M68K_REG_D[2] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_D3: M68K_REG_D[3] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_D4: M68K_REG_D[4] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_D5: M68K_REG_D[5] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_D6: M68K_REG_D[6] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_D7: M68K_REG_D[7] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_A0: M68K_REG_A[0] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_A1: M68K_REG_A[1] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_A2: M68K_REG_A[2] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_A3: M68K_REG_A[3] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_A4: M68K_REG_A[4] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_A5: M68K_REG_A[5] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_A6: M68K_REG_A[6] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_A7: M68K_REG_A[7] = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_PC: M68K_JUMP(M68K_MASK_OUT_ABOVE_32(VALUE)); return;
        case M68K_SR: M68K_SET_SR(VALUE); return;
        case M68K_SP: M68K_REG_SP = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        case M68K_IR: M68K_REG_IR = M68K_MASK_OUT_ABOVE_32(VALUE); return;
        default: break;
    }
}

/*===================================================================================*/
/*                                  68000 CALLBACKS                                 */
/*==================================================================================*/

/* THE FOLLOWING ARE A BUNCH OF ARBITRARY CALLBACKS PERTAINING TOWARDS THE CPU */

#undef M68K_CPU_ARGS

/* DEFINE THE BASE JUMP COROUTINE TO BE ABLE TO DISCERN */
/* AS AND WHEN A NEW CONDITION IS RECOGNISED */ 

/* GETS THE CURRENT VALUE AT THE PC THAT IS BEING EXECUTED */
/* HANDLES THE JUMP HOOK BY DETERMINING THE PREVIOUS */

void M68K_JUMP(unsigned NEW_PC)
{
    M68K_REG_PPC = NEW_PC;
}

void M68K_JUMP_VECTOR(unsigned VECTOR)
{
    M68K_REG_PC = M68K_READ_32(VECTOR << 2);
    M68K_BASE_JUMP_HOOK(M68K_REG_PC, M68K_REG_PPC);
}

/* PERFORM A LOGICAL BITWIS AND FROM THE SOURCE DESTINATION */
/* BEING THE VALUE OF THE CURRENT STATUS REGISTER INTERRUPT */
/* STORE THE RESULT IN THE TRACE REGISTERS */

/* SEE: https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf#page=119 */

void M68K_SET_SR_IRQ(unsigned VALUE)
{
	VALUE &= M68K_SR_MASK;
	M68K_FLAG_T1 += ((S8)VALUE);
}

// GET AND SET THE NEW SP TO THE CURRENT S FLAG
void M68K_SET_S_FLAG(unsigned VALUE)
{
    M68K_FLAG_S = VALUE;
    M68K_REG_SP = M68K_FLAG_S;
}

void M68K_SET_SR(unsigned VALUE)
{
    // https://en.wikibooks.org/wiki/68000_Assembly/Registers
    M68K_FLAG_T1 = M68K_BIT_F(VALUE);

    M68K_SET_CCR(VALUE);
    M68K_SET_S_FLAG(M68K_BIT_SHIFT_16(VALUE));
}

// SET THE CCR WITH THE APPROPRIATE BIT MASKS VALUES
// WHICH ARE INDICATIVE OF THE RANGE THEY ENCOMPASS
void M68K_SET_CCR(unsigned VALUE)
{
    // https://stackoverflow.com/questions/48826709/motorola-68k-understanding-the-status-registrer-flag-states
    M68K_FLAG_X = M68K_BIT_4(VALUE) << 4;
    M68K_FLAG_N = M68K_BIT_3(VALUE) << 3;
    M68K_FLAG_Z = !M68K_BIT_2(VALUE) << 2;
    M68K_FLAG_V = M68K_BIT_1(VALUE) << 1;
    M68K_FLAG_C = M68K_BIT_0(VALUE);
}

// MAP ALL OF THE CORRESPONDING ADDRESSABLE SPACE ACCORDINGLY
void M68K_INIT(void)
{      
    M68K_BUILD_OPCODE_TABLE();

    // 512KB RAM 
    MEMORY_MAP(0x000000, 0x07FFFF, true);
    
    // 512KB ROM 
    MEMORY_MAP(0x080000, 0x0FFFFF, false);            
}

// EXEC FUNCTION STRICTLY DESIGNED FOR THE PURPOSE OF THE SIMULATOR
// TYPICALLY IN ANY OTHER CONTEXT, THERE WONT BE A HARD-CODED AMOUNT OF 
// CYCLES TO REACH BEFORE THE PROGRAM HALTS

void M68K_EXEC(int CYCLES) 
{   
    while(!M68K_CPU_STOPPED)
    {
        // SANITY CHECK TO MAKE SURE THE
        // CURRENT PC HASNT ADVANCED TOO EARLY
        M68K_REG_PPC = M68K_REG_PC;

        // WE DONT WANT TO ACTUALLY DO AN IMMEDIATE READ ON THE PC
        // THIS IS BECAUSE THAT IF WE IMMEDIATELY AUTO-INCREMENT,
        // THEN IT WILL CAUSE THE VERY FIRST OPCODE TO GO UNRECOGNISED

        // 03/07/25 - DOING AN IMMEDIATE READ AGAINST AN OPCODE WHICH
        // REQUIRES AN IMM READ FOR IT'S EXTENSION WILL CAUSE THIS
        // TO PRODUCE A BAD READ - STICK TO READING THE PC INSTEAD

        M68K_REG_IR = M68K_READ_16(M68K_REG_PC);

        // CYCLE RANGE IS MORE SO DETERMINED BY THE MAX AMOUNT OF MEMORY ON THE BUS
        CYCLES = CYCLE_RANGE[M68K_REG_IR];

        printf("[PC -> %04X]  [IR -> %04X]  ", M68K_REG_PC, M68K_REG_IR);
        
        M68K_OPCODE_JUMP_TABLE[M68K_REG_IR]();

        M68K_REG_PC += 2;
        M68K_USE_CYCLES(CYCLE_RANGE[M68K_REG_IR]);
        
        printf("CYCLES: %d, TOTAL ELAPSED: %d\n", CYCLES, M68K_MASTER_CYC);
        printf("-------------------------------------------------------------\n");
    }

    // SET PPC TO NEXT ENTRY IN THE EXEC
    M68K_REG_PPC = M68K_REG_PC;

    printf("EXECUTION STOPPED AT 0x%04X\n", M68K_REG_PC);
    printf("TOTAL CYCLES USED: %d\n", M68K_MASTER_CYC);
}
