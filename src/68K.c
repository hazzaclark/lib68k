/* COPYRIHGT (C) HARRY CLARK 2025 */

/* MOTOROLA 68000 STANDALONE EMULATION LIBRARY */

/* THIS FILE PERTAINS TOWARDSS THE MAIN FUNCTIONALITY SURROUNDING THE UTILITY */
/* OF THE CPU AND IT'S SUBSEQUENT DIRECTIVES AND DEFINES */

/* ANY AND ALL OF THE CORRESPONDING DEFINES AND DECLARATIONS CAN BE */
/* ATTRIBUTED TO THE INFROMATION FOUND HERE: https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf  */ 

#ifndef USE_OPCODE_HANDLER_TABLE

/* NESTED INCLUDES */

#include "68K.h"
#include "68KCONF.h"
#include "common.h"
#include "util.h"

#endif

#ifndef USE_68K
#ifdef BUILD_OP_TABLE

int M68K_INITIAL_CYCLES;
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

        default: return 0;
    }
}

/* IN A SIMILAR VEIN TO THE FUNCTION ABOVE, THIS FUNCTION WILL FOCUS MOREOVER */
/* ON THE SETTING OF THESE VALUES */

void M68K_SET_REGISTERS(unsigned int REGISTER, unsigned int VALUE)
{
    switch (REGISTER)
    {
        case M68K_D0: CPU.DATA_REGISTER[0] = VALUE; break;
        case M68K_D1: CPU.DATA_REGISTER[1] = VALUE; break;
        case M68K_D2: CPU.DATA_REGISTER[2] = VALUE; break;
        case M68K_D3: CPU.DATA_REGISTER[3] = VALUE; break;
        case M68K_D4: CPU.DATA_REGISTER[4] = VALUE; break;
        case M68K_D5: CPU.DATA_REGISTER[5] = VALUE; break;
        case M68K_D6: CPU.DATA_REGISTER[6] = VALUE; break;
        case M68K_D7: CPU.DATA_REGISTER[7] = VALUE; break;
        case M68K_A0: CPU.ADDRESS_REGISTER[0] = VALUE; break;
        case M68K_A1: CPU.ADDRESS_REGISTER[1] = VALUE; break;
        case M68K_A2: CPU.ADDRESS_REGISTER[2] = VALUE; break;
        case M68K_A3: CPU.ADDRESS_REGISTER[3] = VALUE; break;
        case M68K_A4: CPU.ADDRESS_REGISTER[4] = VALUE; break;
        case M68K_A5: CPU.ADDRESS_REGISTER[5] = VALUE; break;
        case M68K_A6: CPU.ADDRESS_REGISTER[6] = VALUE; break;
        case M68K_A7: CPU.ADDRESS_REGISTER[7] = VALUE; break;
        case M68K_PC: CPU.PC = VALUE; break;
        case M68K_SR: CPU.STATUS_REGISTER = VALUE; break;
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

void M68K_JUMP()
{
    U32 FROM_ADDR = M68K_REG_PPC;
    M68K_REG_PPC = FROM_ADDR;
    M68K_REG_PC = M68K_REG_JMP_TARG;

    M68K_BASE_JUMP_HOOK(M68K_REG_JMP_TARG, FROM_ADDR);

    // RESET FOR NEW COND.
    M68K_REG_JMP_TARG = 0;
}

void M68K_JUMP_VECTOR(unsigned VECTOR)
{
    M68K_REG_PC = M68K_READ_16(VECTOR << 2);
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
    M68K_FLAG_T1 = M68K_BIT_F(VALUE);
    M68K_SET_CCR(VALUE);
    M68K_SET_S_FLAG((VALUE >> 15) & 1);
}

void M68K_SET_CCR(unsigned VALUE)
{
    // SOURCE: https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf#page=14
    M68K_FLAG_X = M68K_BIT_4(VALUE) << 4;
    M68K_FLAG_N = M68K_BIT_3(VALUE) << 4;
    M68K_FLAG_Z = M68K_BIT_2(VALUE);
    M68K_FLAG_V = M68K_BIT_1(VALUE) << 6;
    M68K_FLAG_C = M68K_BIT_0(VALUE) << 8;
}

void M68K_INIT(void)
{    
    M68K_REG_PC = 0x00001000;
    M68K_REG_PPC = 0x00000000;
    M68K_REG_IR = 0x00000000;
    M68K_REG_SP = 0x1000;

    M68K_BUILD_OPCODE_TABLE();

    // MAP ALL OF THE CORRESPONDING ADDRESSABLE SPACE ACCORDINGLY
    // THE 24-BIT ADDRESSABLE SPACE ON THE BUS REQUIRES AT A MINIMUM
    // THE SPACE TO ALLOCATE RAM AND SYSTEM VECTORS

    // ROM AND IO COME ABOUT WHEN EMULATING ON-BOARD/EXT. HARDWARE
    // SUCH AS OTHER CHIPS AND SEPERATING PROGRAM CODE FROM ADDRESSABLE RAM

    // https://www.cpcwiki.eu/imgs/7/7a/MC68000_User%27s_Manual.pdf
    // https://www.cpcwiki.eu/index.php/Motorola_68000

    // 512KB (RAM)
    MEMORY_MAP(0x000000, 0x80000, true);   
    
    // 512KB (ROM)
    MEMORY_MAP(0x400000, 0x80000, false);  
    
    // 64KB (IO)
    MEMORY_MAP(0xF00000, 0x10000, true);   
    
    // 64KB (VECTORS)
    MEMORY_MAP(0xFFFF0000, 0x10000, false);

    // TODO: WORK ON THE MEMORY BUS FOR TYPES 68020 ONWARDS
    
    M68K_SET_CPU_TYPE(CPU_TYPE);
}

// EXEC FUNCTION STRICTLY DESIGNED FOR THE PURPOSE OF THE SIMULATOR
// TYPICALLY IN ANY OTHER CONTEXT, THERE WONT BE A HARD-CODED AMOUNT OF CYCLES TO REACH BEFORE THE PROGRAM HALTS

int M68K_EXEC(int CYCLES) 
{
    M68K_INITIAL_CYCLES = CYCLES;
    M68K_SET_CYCLES(CYCLES);
    
    printf("M68K SETUP WITH CYCLES %d\n", CYCLES);
    printf("------------------------------------------------------------\n");

    while(!M68K_CPU_STOPPED && CPU.MASTER_CYCLES > 0)
    {
        M68K_REG_PPC = M68K_REG_PC;

        // WE DONT WANT TO ACTUALLY DO AN IMMEDIATE READ ON THE PC
        // THIS IS BECAUSE THAT IF WE IMMEDIATELY AUTO-INCREMENT,
        // THEN IT WILL CAUSE THE VERY FIRST OPCODE TO GO UNRECOGNISED

        M68K_REG_IR = M68K_READ_16(M68K_REG_PC);

        int CURRENT_CYCLES = CYCLE_RANGE[M68K_REG_IR];

        if (CPU.MASTER_CYCLES < CURRENT_CYCLES) 
        {
            CPU.MASTER_CYCLES = 0;
            break;
        }

        printf("[PC -> %04X]  [IR -> %04X]  ", M68K_REG_PC, M68K_REG_IR);

        M68K_OPCODE_JUMP_TABLE[M68K_REG_IR]();

        // COMPLETELY BYPASSES THE NEED FOR M68K_USE_CYCLES AS WE DONT
        // WANT THE REMAININING CYCLES BEING SUBTRACTED FROM THE CURRENT

        // NOT TO MENTION THE VARYING SIGNEDNESS

        // 14/05/25 - NOTE THAT WHEN IT COMES TO WORKING WITH SYSTEMS
        // THE CYCLE COUNT WILL INCREMENT AS OPPOSED TO DECREMENT

        // THIS IS ESPECIALLY APPARENT WHEN WORKING WITH THE MEGA DRIVE
        // AND ACCOUNTING FOR H/VBLANK CYCLES FOR THE VDP AS AN EXAMPLE

        // QUITE SELF EXPLANATORY BUT, JUST REMEMBER THAT SO YOU AREN'T
        // SCRATCHING YOUR HEAD TRYING TO GET THE EXEC LOOP WORKING ON
        // BOTH THIS AND THE MD EMULATOR 

        M68K_REG_PC += 2;
        CPU.MASTER_CYCLES -= CURRENT_CYCLES;
        
        printf("CYCLE: %d, REMAINING: %d\n", CURRENT_CYCLES, CPU.MASTER_CYCLES);
        printf("-------------------------------------------------------------\n");
    }

    // SET PPC TO NEXT ENTRY IN THE EXEC
    M68K_REG_PPC = M68K_REG_PC;

    printf("EXECUTION STOPPED AT 0x%04X\n", M68K_REG_PC);
    printf("TOTAL CYCLES USED: %d\n", M68K_INITIAL_CYCLES - CPU.MASTER_CYCLES);
    printf("CYCLES REMAINING: %d\n", CPU.MASTER_CYCLES);

    return M68K_INITIAL_CYCLES - CPU.MASTER_CYCLES;
}

#endif
#endif
