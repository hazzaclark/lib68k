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

#endif

#ifndef USE_68K

/*===============================================================================*/
/*							68000 MAIN CPU FUNCTIONALIY							 */
/*===============================================================================*/

/* FOLLOWING THE INITIAL DECLARATION OF THE VECTOR TABLE */
/* DISCERN THE RUDIMENTARY AMOUNT OF CPU CYCLES */
/* THE CPU WILL GOVERN OVER THE COURSE OF IT'S RUN TIME */

/* THIS WILL ENCOMPASS THE WIDE BITWISE VARIETY */
/* THROUGH THE MEANS OF EVALUATING THE SIZE OF THE DESIGNATED MEMORY */

/* SEE 1.1 USER PROGRAMMING MODEL https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf#page=13 */

void INITIALISE_68K_CYCLES(void)
{
	char* CPU_68K_CYCLES = (char*)malloc(sizeof((char)M68K_BASE_BITMASK));

	/* THIS LOOP WILL CHECK FOR EVERY SUBSEQUENT BITWISE */
	/* OPERATION AND EVALUATE IT'S DESIGNATED MEMORY */

	/* THE FIRST CO-EFFICIENT REPRESENTS THE BITWISE LENGTH OF THE OPERATION */
	/* THE SECOND CO-EFFICIENT REPRESENTS THE SIZE OF THE REGISTER */

	for (size_t INDEX = 0; INDEX < sizeof(CPU_68K_CYCLES); INDEX++)
	{	
		switch (INDEX / 16)
		{
			case 0:
				CPU_68K_CYCLES[INDEX] += (int)M68K_LOW_BITMASK;
				break;

			case 1:
				CPU_68K_CYCLES[INDEX] += (int)M68K_MID_BITMASK;
				break;

			case 2:
				CPU_68K_CYCLES[INDEX] += (int)M68K_HIGH_BITMASK;
				break;

			default:
				CPU_68K_CYCLES[INDEX] += (int)M68K_MAX_BITMASK;
				break;
		}
	}
}

/* ACCESS EACH RESPECTIVE REGISTER FROM THE ENUMERATION */
/* RETURN THE CORRESPONDENCE IN RELATION TO THE SIZE */

/* FOR EASE OF USE WHEN ACCESSING METHODS, I HAVE CREATED A MACRO */
/* TO BE ABLE TO RETURN THE CORRESPONDING ADDRESS VALUE IN RELATION */
/* TO THE INDEXXING OF THE REGISTER ARRAY */

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

        default: return 0;
    }
}

/* IN A SIMILAR VEIN TO THE FUNCTION ABOVE, THIS FUNCTION WILL FOCUS MOREOVER */
/* ON THE SETTING OF THESE VALUES */

/* THIS IS POSSIBLE DUE TO THE WAY IN WHICH THE ENUM VALUES ARE ASSIGNED TO THE SPECIFIC */
/* MACRO WITH AN ALLOCATED SIZE */

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

/*===============================================================================*/
/*									68000 CALLBACKS							     */
/*===============================================================================*/

/* THE FOLLOWING ARE A BUNCH OF ARBITRARY CALLBACKS PERTAINING TOWARDS THE CPU */

/* INTIALISE THE INTEGER TYPE CALLBACK */
/* THIS CALLBACK REFERS TO THE WAY IN WHICH THE DESIGNATED TYPE */
/* IS ABLE TO COMMUNICATE BETWEEN ALL AREAS OF EACH RESPECTIVE REGISTER */

/* SEE: https://www.nxp.com/docs/en/reference-manual/M68000PRM.pdf#page=14 */

/* ALLOCATE A DESIGNATED CHUNK OF MEMORY ASSUMING THE NOTION */
/* THAT THE RESPECTIVE REGISTER NEEDS TO ACCESS VALUES OF THIS ASSOCIATION */

#undef M68K_CPU_ARGS

int M68K_DEFAULT_INT_CALLBACK;
int M68K_SET_INT_CALLBACK(int LEVEL)
{
	M68K_DEFAULT_INT_CALLBACK = LEVEL;
	M68K_FLAG_INT_LVL = 0;
	return M68K_REG_VBR;
}

/* FOLLOWING ON FROM THE SAME PRINCIPLES AS ABOVE */
/* INITIALISE THE REMAINING CALLBACKS */

void M68K_SET_FUNC_CALLBACK(void (*CALLBACK)(void))
{
    M68K_SET_FC_ACK = CALLBACK ? CALLBACK : 0;
}

unsigned int M68K_INSTR_CALLBACK;
void M68K_SET_INSTR_CALLBACK(unsigned CALLBACK, unsigned PC)
{
	M68K_INSTR_CALLBACK = CALLBACK &= PC;
}

/* DEFINE THE BASE JUMP COROUTINE TO BE ABLE TO DISCERN */
/* AS AND WHEN A NEW CONDITION IS RECOGNISED */ 

void M68K_JUMP(unsigned NEW_PC)
{
	M68K_REG_PC += (int)NEW_PC;
}

void M68K_JUMP_VECTOR(unsigned VECTOR)
{
	M68K_REG_PC += M68K_READ_32(VECTOR << 2);
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

void M68K_INIT(void)
{    
    for (int i = 0; i < 8; i++) 
    {
        CPU.DATA_REGISTER[i] = i;
        CPU.ADDRESS_REGISTER[i] = i + 8;
    }
    
    CPU.STATUS_REGISTER = 0x2000;
    CPU.PC = 0x0000;

    printf("INITIALISING OPCODE TABLE...\n");
    M68K_BUILD_OPCODE_TABLE();
    printf("OPCODE TABLE INITIALISED.\n");

    printf("SETTING INTERRUPT CALLBACK...\n");
    M68K_SET_INT_CALLBACK(0);
    printf("INTERRUPT CALLBACK SET.\n");

    printf("SETTING FUNCTION CALLBACK...\n");
    M68K_SET_FUNC_CALLBACK(0);
    printf("FUNCTION CALLBACK SET.\n");

    printf("SETTING INSTRUCTION CALLBACK...\n");
    M68K_SET_INSTR_CALLBACK(0, 0);
    printf("INSTRUCTION CALLBACK SET.\n");

	INITIALIZE_MEMORY();

    printf("68000 INITIALISATION COMPLETE.\n");
}

// EXEC FUNCTION STRICTLY DESIGNED FOR THE PURPOSE OF THE SIMULATOR
// TYPICALLY IN ANY OTHER CONTEXT, THERE WONT BE A HARD-CODED AMOUNT OF CYCLES TO REACH BEFORE THE PROGRAM HALTS

int M68K_EXEC(int CYCLES)
{
    if(M68K_REG_PC == 0)
    {
        M68K_REG_PC = 0x00000000;
    }

    printf("INIT PC: 0x%08X\n", M68K_REG_PC);

    if(M68K_RESET_CYCLES > 0)
    {
        int RC = M68K_RESET_CYCLES;
        M68K_RESET_CYCLES = 0;
        CYCLES -= RC;
        if(CYCLES <= 0) return RC; 
    }

    int INIT_CYCLES = CYCLES;
    int REMAINING_CYCLES = CYCLES;
    M68K_CYC_REMAIN = REMAINING_CYCLES;

    if(!M68K_CPU_STOPPED)
    {
        int SAFETY_COUNTER = 0;
        const int MAX_INSTR = 10000; 

        while(REMAINING_CYCLES > 0 && SAFETY_COUNTER++ < MAX_INSTR)
        {
            printf("PC BEFORE FETCH: 0x%08X\n", M68K_REG_PC);

            M68K_REG_PPC = M68K_REG_PC;
            M68K_REG_IR = READ_IMM_16();
            
            M68K_REG_PC += 2;
            
            printf("PC AFTER FETCH 0x%08X, Instruction: 0x%04X\n", 
                   M68K_REG_PC, M68K_REG_IR);
            
            if(M68K_REG_IR) 
            {
                if(M68K_OPCODE_JUMP_TABLE[M68K_REG_IR])
                {
                    M68K_OPCODE_JUMP_TABLE[M68K_REG_IR]();
                }

                else
                {
                    M68K_REG_PC += 2;
                    continue;
                }
                
                int INSTR_CYCLES = 4; 
                if(CPU.INSTRUCTION_CYCLES && CPU.INSTRUCTION_CYCLES[M68K_REG_IR] > 0)
                {
                    INSTR_CYCLES = CPU.INSTRUCTION_CYCLES[M68K_REG_IR];
                }

                REMAINING_CYCLES -= INSTR_CYCLES;
                M68K_CYC_REMAIN = REMAINING_CYCLES;
            }
            else
            {
                fprintf(stderr, "ZERO INSTRUCTION AT PC=0x%08X\n", M68K_REG_PPC);
                break;
            }
        }

        if(SAFETY_COUNTER >= MAX_INSTR)
        {
            fprintf(stderr, "MAX INSTR LIMIT REACHED PC=0x%08X\n",
                   M68K_REG_PC);
        }
    }

    printf("EXEC COMPLETE, FINAL PC: 0x%08X\n", M68K_REG_PC);
    return INIT_CYCLES - REMAINING_CYCLES;
}

#endif
