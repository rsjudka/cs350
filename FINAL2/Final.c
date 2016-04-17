//Robert Judka
// CS 350, Spring 2016
// Final Project, part 2

#include <stdio.h>
#include <stdlib.h>       // For error exit()
#include <string.h>
#include <stddef.h>

	typedef unsigned short int Word;
	typedef unsigned short int Address; 
	typedef unsigned short cmd;

	#define MEMLEN 65536
	#define NREG 8

	typedef struct {
		Word mem[MEMLEN];
		Word orig;
		Word reg[NREG];		//array holding registers 1-8
		Address pc;			//Program Counter
		int running;			//running = 1 iff CPU is executing instructions
		Word ir;				//Instruction Register
		char cc;				//Conditional Code
        int opcode;				//Operation to execute
		int reg_R;				//Registry field
		int addr_MM;			//Memory field
		int prevDst;
	} CPU;
		
// Prototypes
	int main(int argc, char *argv[]);
	int read_execute_command(CPU *cpu);
    int execute_command(char cmd_char, CPU *cpu);
    void help_message(void);
    void many_instruction_cycles(int nbr_cycles, CPU *cpu);
    void one_instruction_cycle(CPU *cpu);
	
	int read_char();

    void exec_HLT(CPU *cpu);
	void exec_INSTR(CPU *cpu);
	void initialize_CPU(CPU *cpu);
	void initialize_memory(int argc, char *argv[], CPU *cpu);
	FILE *get_datafile(int argc, char *argv[]);

	void dump_CPU(CPU *cpu);
	void dump_memory(CPU *cpu);
	void dump_registers(CPU *cpu);

//Main program: Initialize the cpu, and read the initial memory values
int main(int argc, char *argv[]) {
	printf("Robert Judka CS350-01\n");
	printf("LC-3 Simulator: Final Project pt. \n");
	CPU cpu_value, *cpu = &cpu_value;
	initialize_CPU(cpu);
	initialize_memory(argc, argv, cpu);
	printf("\nINITIAL CPU:\n");	
	dump_CPU(cpu);
	printf("\n");
	dump_memory(cpu);
	
	char *prompt = "> ";
    printf("\n\nWaiting on your call mate!\n\tType 'h' or '?' for help\n%s", prompt);

    int done = read_execute_command(cpu);
    while (!done) {
        printf("%s", prompt);
        done = read_execute_command(cpu);
    }
	return 0;
}

int read_execute_command(CPU *cpu) {
// Buffer to read next command line into
#define CMD_LINE_LEN 80
    char cmd_line[CMD_LINE_LEN];
    char *read_success;     // NULL if reading in a line fails.

    int nbr_cycles;     // Number of instruction cycles to execute
    char cmd_char;      // Command 'q', 'h', '?', 'd', or '\n'
    int done = 0;       // Should simulator stop?
	
	int value_read, words_read, loc = 0;

    read_success = fgets(cmd_line, CMD_LINE_LEN, stdin);
	
	////used to distinguish between char, int, and EOF
	value_read = cmd_line[loc];
	words_read = sscanf(cmd_line, "%d", &value_read);
    // *** STUB ***  Quit if read_success indicates end-of-file
	if (read_success == NULL)
	{
		done = 1;
	}
	
	// *** STUB ***
	// while not done,
	//Handle q, h, ?, d commands, integer (nbr of instruction cycles),
	//or empty line (one instruction cycle)
	//Read next command line	
	if (words_read <= 0)
	{
		cmd_char = *read_success;		
		done = execute_command(cmd_char, cpu);
	}
	else
	{
		nbr_cycles = value_read;
		many_instruction_cycles(nbr_cycles, cpu);
	}

    return done;
}

// Execute a nonnumeric command; complain if it's not 'h', '?', 'd', 'q' or '\n'
// Return true for the q command, false otherwise
//
int execute_command(char cmd_char, CPU *cpu) {
    if (cmd_char == '?' || cmd_char == 'h')
	{
        help_message();
    }
    // *** STUB ****
	else if (cmd_char == 'q')
	{
		printf("Now quiting. Goodbye.\n");
		return 1;
	}
	else if (cmd_char == 'd')
	{
		printf("\nCPU:\n");
		dump_CPU(cpu);
		printf("\n");
		//dump_memory(cpu);
		printf("\n");
	}
	else if (cmd_char == '\n')
	{
		one_instruction_cycle(cpu);
	}
	else
	{
		printf("no such thing!\n");
	}
    return 0;
}

// Print standard message for simulator help command ('h' or '?')
//
void help_message(void) {
    // *** STUB ****
	printf("\nStuck in a pickle? Give us a few minutes, a representative will be with you shortly!\n");
	for (int i = 0; i < 30; i++)
	{
		printf(".");
		int j = 0;
		while (j < 1e7*2)
		{
			j++;
		}
		if (i == 10 || i == 20)
		{
			printf("\r           ");
			printf("\r");
			//for (int x = 0; x < i; x++)
			//{
				//printf
		}
	}
	printf("\n\nJK ain't nobody got time for dat!\n");
	printf("\nHere is a list of your options:\n");
	printf("\t'd' - dumps out CPU\n");
	printf("\t'h' or '?' - help menu (seems kinda redundant at this point)\n");
	printf("\t'q' - quits the program\n");
	printf("\tan integer > 0 - executes N instruction cycles (don't get too crazy with this one!)\n");
	printf("\treturn - executes one instruction cycle\n");
	
	/*printf("\nSDC Instruction Set:");
	printf("\n   0xxx: HALT");
	printf("\n   1RMM: Load Reg[R] <- M[MM]");
	printf("\n   2RMM: Store M[MM] <- Reg[R]");
	printf("\n   3RMM: Add M[MM] to Reg[R]");
	printf("\n   4Rxx: Negate Reg[R]");
 	printf("\n   5RMM: Load Immediate Reg[R] <- MM");
	printf("\n  -5RMM: Load Immediate Reg[R] <- -MM");
 	printf("\n   6RMM: Add Immediate Reg[R] <- Reg[R] + MM");
	printf("\n  -6RMM: Subtract Immediate Reg[R] <- Reg[R] - MM");
 	printf("\n   7xMM: Branch to MM");
 	printf("\n   8RMM: Branch Conditional to MM");
 	printf("\n   90xx: Read char into R0");
 	printf("\n   91xx: Print char in R0");
 	printf("\n   92MM: Print string starting at MM");
 	printf("\n   93MM: Dump CPU");
 	printf("\n   94MM: Dump memory\n\n");
	printf("Waiting on your call mate!\n");*/

}

// Execute a number of instruction cycles.  Exceptions: If the
// number of cycles is <= 0, complain and return; if the CPU is
// not running, say so and return; if the number of cycles is
// insanely large, complain and substitute a saner limit.
//
// If, as we execute the many cycles, the CPU stops running,
// stop and return.
//
void many_instruction_cycles(int nbr_cycles, CPU *cpu) {
    int INSANE_NBR_CYCLES = 100;

    // *** STUB ****
	if (nbr_cycles <= 0)
	{
		printf("Sorry mate, its imposible to do %d instruction cycles...\nJust think about it\n", nbr_cycles);
		return;
	}
	else if ( nbr_cycles >= INSANE_NBR_CYCLES)
	{
		printf("You crazy! %d too many instruction cycles, we'll make it a SANE value for you\n", nbr_cycles);
		nbr_cycles = 99;
	}
	
	if ((*cpu).running != 1)
	{
		printf("HALTED! Sorry CPU is not running, he gave up around mile 3 :/\n");
	}
	else
	{
		for (int i = 0; i < nbr_cycles; i++)
		{
			if ((*cpu).running == 1)
			{
				one_instruction_cycle(cpu);
			}
			else
			{
				break;
			}
		}
	}
}


// Execute one instruction cycle
//
void one_instruction_cycle(CPU *cpu) {
    // If the CPU isn't running, say so and return.
    // If the pc is out of range, complain and stop running the CPU.
    //
    // *** STUB ****
	if ((*cpu).mem[(*cpu).pc] == 0)
	{
		printf("%d", (*cpu).mem[(*cpu).pc]);
		(*cpu).running = 0;
	}
	if ((*cpu).running == 0)
	{
		printf("HALTED! Sorry CPU is not running, he gave up around mile 3 :/\n");
		return;
	}
	if (!((*cpu).pc >= 0 && (*cpu).pc <= MEMLEN))
	{
		printf("Sorry PC out of range! You broke him :(\n");
		return;
	}
	
		//go through one statement
		printf("x%.4hX: ", (*cpu).pc);
		
		Address temp_mem = (*cpu).mem[(*cpu).pc];
		int next_loc = (*cpu).pc + 1;
		
		int instr_loc = (*cpu).pc;
		cpu -> ir = (*cpu).mem[(*cpu).pc]; 
		(cpu -> pc)++;
		
		//flag variables for signed or unsigned values
		cmd imm5Flag;
		cmd jsrFlag;
		cmd brFlag;
		
		//masks to save flag bit
		imm5Flag = temp_mem & 0x0020;
		jsrFlag = temp_mem & 0x0800;
		brFlag = temp_mem & 0x0E00;
		
		//variables for parts of the bitstring
		cmd opCode;
		cmd dst;
		cmd src1;
		cmd src2;
		cmd base;
		cmd trap;
		
		//shifts to remove excess bits
		cmd shift4 = (temp_mem << 4); 
		cmd shift7 = (temp_mem << 7);
		cmd shift8 = (temp_mem << 8);
		cmd shift13 = (temp_mem << 13);
		
		//shifts to set value according to shifts
		opCode = (temp_mem >> 12);
		cpu -> opcode = opCode;
		dst = (shift4 >> 13);
		src1 = (shift7 >> 13);
		src2 = (shift13 >> 13);
		base = (shift7 >> 13);
		trap = (shift8 >> 8);
		
		//variables for signed parts of the bitstring (where +- is important)
		signed short imm5;
		signed short PCoffset9;
		signed short PCoffset11;
		signed short LDRoffset;
		
		//shifts to remove excess bits
		signed short sShift5 = (temp_mem << 5);
		signed short sShift7 = (temp_mem << 7);
		signed short sShift10 = (temp_mem << 10);
		signed short sShift11 = (temp_mem << 11);
		
		//shifts to set value according to shifts
		imm5 = (sShift11 >> 11);
		PCoffset9 = (sShift7 >> 7);
		PCoffset11 = (sShift5 >> 5);
		LDRoffset = (sShift10 >> 10);
		
		//output according to opcode
		int tempOffset = 0;
		Address target = 0;
		switch ((opCode))
		{
			case  0:
				PCoffset9 += next_loc;
				if (brFlag == 0x0800)
				{
					printf("x%.4hX\t% d\tBRN\tx%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], PCoffset9);
					if ((*cpu).prevDst < 0)
					{
						(*cpu).pc += PCoffset9;
					}
				}
				else if (brFlag == 0x0400)
				{
					printf("x%.4hX\t% d\tBRZ\tx%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], PCoffset9);
					if ((*cpu).prevDst == 0)
					{
						(*cpu).pc += PCoffset9;
					}
				}
				else if (brFlag == 0x0200)
				{
					printf("x%.4hX\t% d\tBRP\tx%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], PCoffset9);
					if ((*cpu).prevDst > 0)
					{
						(*cpu).pc += PCoffset9;
					}
				}
				else if (brFlag == 0x0C00)
				{
					printf("x%.4hX\t% d\tBRNZ\tx%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], PCoffset9);
					if ((*cpu).prevDst <= 0)
					{
						(*cpu).pc += PCoffset9;
					}
				}
				else if (brFlag == 0x0A00)
				{
					printf("x%.4hX\t% d\tBRNP\tx%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], PCoffset9);
					if (((*cpu).prevDst < 0) || (*cpu).prevDst > 0)
					{
						(*cpu).pc += PCoffset9;
					}
				}
				else if (brFlag == 0x0600)
				{
					printf("x%.4hX\t% d\tBRZP\tx%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], PCoffset9);
					if ((*cpu).prevDst >= 0)
					{
						(*cpu).pc += PCoffset9;
					}
				}
				else if (brFlag == 0x0E00)
				{
					printf("x%.4hX\t% d\tBRNZP\tx%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], PCoffset9);
					(*cpu).pc += PCoffset9;
				}
				else
				{
					printf("x%.4hX\t% d\tNOP\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc]);
				}
				break;
			case 1:
				if (imm5Flag == 0x0020)
				{
					printf("x%.4hX\t% d\tADD\tR%d, R%d, #%d\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, src1, imm5);
					(*cpu).reg[dst] = (*cpu).reg[src1] + imm5;
					(*cpu).prevDst = dst;
				}
				else if(imm5Flag == 0x0000)
				{
					printf("x%.4hX\t% d\tADD\tR%d, R%d, R%d\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, src1, src2);
					(*cpu).reg[dst] = (*cpu).reg[src1] + (*cpu).reg[src2];
					(*cpu).prevDst = dst;
				}	
				break;
			case 2:
				tempOffset = PCoffset9;
				PCoffset9 += next_loc;
				printf("x%.4hX\t% d\tLD\tR%d, x%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, PCoffset9);
				(*cpu).reg[dst] = (*cpu).mem[(*cpu).pc + tempOffset];
				(*cpu).prevDst = dst;
				break;
	 		case 3:
				PCoffset9 += next_loc;
				//dst is same as src for ST
				printf("x%.4hX\t% d\tST\tR%d, x%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, PCoffset9);
				(*cpu).mem[(*cpu).pc + PCoffset9] = (*cpu).reg[dst];
				(*cpu).prevDst = dst;
				break;
			case 4:
				if (jsrFlag == 0x0800)
				{
					PCoffset11 += next_loc;
					printf("x%.4hX\t% d\tJSR\tx%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], PCoffset11);
					(*cpu).reg[7] = (*cpu).pc;
					(*cpu).pc += PCoffset11;
					(*cpu).prevDst = dst;
				}
				else if(jsrFlag == 0x0000)
				{
					printf("x%.4hX\t% d\tJSRR\tR%d\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], base);
					target = (*cpu).reg[base];
					(*cpu).reg[7] = (*cpu).pc;
					(*cpu).pc = target;
					(*cpu).prevDst = dst;
				}
				break;
			case 5:
				if (imm5Flag == 0x0020)
				{
					printf("x%.4hX\t% d\tAND\tR%d, R%d, #%d\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, src1, imm5);
					(*cpu).reg[dst] = (*cpu).reg[src1] && imm5; 
					(*cpu).prevDst = dst;
				}
				else if(imm5Flag == 0x0000)
				{
					printf("x%.4hX\t% d\tAND\tR%d, R%d, R%d\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, src1, src2);
					(*cpu).reg[dst] = (*cpu).reg[src1] && (*cpu).reg[src2];
					(*cpu).prevDst = dst;
				}
				break;
			case 6:
				printf("x%.4hX\t% d\tLDR\tR%d, R%d, #%d\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, src1, LDRoffset);
				(*cpu).reg[dst] = (*cpu).mem[(*cpu).reg[src1] + LDRoffset];
				(*cpu).prevDst = dst;
				break;
			case 7:
				//dst is same as src for STR
				printf("x%.4hX\t% d\tSTR\tR%d, R%d, #%d\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, src1, LDRoffset);
				(*cpu).mem[(*cpu).reg[src1] + LDRoffset] = (*cpu).reg[dst];
				(*cpu).prevDst = dst;
				break;
			case 8:
				printf("x%.4hX\t% d\tRTI\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc]);
				printf("Return from interrupt error, continuing execution");
				break;
			case 9:
				printf("x%.4hX\t% d\tNOT\tR%d, R%d\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, src1);
				(*cpu).reg[dst] = -(*cpu).reg[src1] - 1;
				(*cpu).prevDst = dst;
				break;
			case 10:
				tempOffset = PCoffset9;
				PCoffset9 += next_loc;
				printf("x%.4hX\t% d\tLDI\tR%d, x%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, PCoffset9);
				(*cpu).reg[dst] = (*cpu).mem[(*cpu).mem[tempOffset + (*cpu).pc]];
				(*cpu).prevDst = dst;
				break;
			case 11:
				PCoffset9 += next_loc;
				//dst is same as src for STI
				printf("x%.4hX\t% d\tSTI\tR%d, x%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, PCoffset9);
				(*cpu).mem[(*cpu).mem[(*cpu).pc + PCoffset9]] = (*cpu).reg[dst];
				(*cpu).prevDst = dst;
				break;
			case 12:
				//src1 same as base for JMP
				if (src1 == 7)
				{
					printf("x%.4hX\t% d\tRET\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc]);
					(*cpu).pc = (*cpu).reg[src1];
				}
				else
				{
					printf("x%.4hX\t% d\tJMP\tR%d\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], src1);
					(*cpu).pc = (*cpu).reg[src1];
				}
				break;
			case 13:
				printf("x%.4hX\t% d\terr\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc]);
				printf("No operation, continuing execution");
				break;
			case 14:
				PCoffset9 += next_loc;
				printf("x%.4hX\t% d\tLEA\tR%d, x%.4hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], dst, PCoffset9);
				(*cpu).reg[dst] = PCoffset9;
				(*cpu).prevDst = dst;
				break;
			case 15:
				if (trap == 0x20)
				{
					char input;
					printf("x%.4hX\t% d\tTRAP\tGETC\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc]);
					printf("type char & return (or just press return): ");
					input = read_char();
					(*cpu).reg[0] = input;
				}
				else if (trap == 0x21)
				{
					printf("x%.4hX\t% d\tTRAP\tOUT\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc]);
					printf("%c\n", (*cpu).reg[0]);
				}
				else if (trap == 0x22)
				{
					printf("x%.4hX\t% d\tTRAP\tPUTS\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc]);
				}
				else if (trap == 0x23)
				{
					printf("x%.4hX\t% d\tTRAP\tIN\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc]);
				}
				else if (trap == 0x25)
				{
					printf("x%.4hX\t% d\tTRAP\tHALT\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc]);
				}
				else
				{
					printf("x%.4hX\t% d\tTRAP\tx%.2hX\n", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc], trap);
				}
				break;
			default:
				printf("BAD OPCODE: %d; quitting\n", opCode);
				(*cpu).running = 0;
				break;
		}			
    }
	
	int read_char()
	{
		char buffer[3] = "";
		fgets(buffer, sizeof buffer, stdin);
		return buffer[0];
	}

// Execute the halt instruction (make CPU stop running)
//
void exec_HLT(CPU *cpu) {
    cpu -> running = 0;
}

//Initialize the CPU (pc, ir, cc, running flag,
void initialize_CPU(CPU *cpu) {
	for (int i = 0; i < NREG; i++)
	{
		cpu -> reg[i] = 0;
	}
	for (int i = 0; i < MEMLEN; i++)
	{
		cpu -> mem[i] = 0;
	}
	cpu -> ir = 0;
	cpu -> cc = 'Z';
	cpu -> pc = 0;
	cpu -> running = 1;
	cpu -> opcode = 0;
}

//Read and dump initial values for memory
void initialize_memory(int argc, char *argv[], CPU *cpu) {
	FILE *datafile = get_datafile(argc, argv);

#define BUFFER_LEN 100

char buffer[BUFFER_LEN];
char *read_success;
char hexCode[5];
int loc = 0;
int addFlag = 0;
int saveOrig = 0;

read_success = fgets(buffer, sizeof(buffer), datafile);

while (read_success != NULL)
{
	if (buffer[0] == '\n' || buffer[0] == ';')
	{
		read_success = fgets(buffer, sizeof(buffer), datafile);
	}
	else
	{
		//saves only the first for characters into a variable
		char *p;
		strncpy(hexCode, buffer, 4);
		hexCode[5] = '\0';
		int toInt = 0;
		toInt = strtoul(hexCode, &p, 16);
		
		//Saves first hex value as the origin
		if (saveOrig == 0)
		{
			(*cpu).orig = toInt;
			loc = (*cpu).orig;
			addFlag = 1;
			saveOrig = 1;
		}
		else //saves all other hex values to mem[loc]
		{
			(*cpu).mem[loc] = toInt;
			addFlag = 0;
		}
		
		//loops to mem location 0 if at last space in mem location
		if (loc == MEMLEN)
		{
			loc = 0;
		}
		
		//incriments location if not the origin
		if (addFlag == 0)
		{
			loc++;
		}
		read_success = fgets(buffer, sizeof(buffer), datafile);
	}
}	
	(*cpu).pc = (*cpu).orig;
	//Initialize rest of memory
	while (loc < MEMLEN)
	{
		cpu -> mem[loc++] = 0;
	}
}

//read file from cmd
FILE *get_datafile(int argc, char *argv[]) {
	char *default_datafile_name = "default.hex";
	char *datafile_name = NULL;

	//set datafile name to argv[1] or default
	if (argv[1] != NULL)
	{
		datafile_name = argv[1];
	}
	else
	{
		datafile_name = default_datafile_name;
	}

	FILE *datafile = fopen(datafile_name, "r");
	printf("\nOpening %s...\n", datafile_name);

	//if file not found, print error and quit
	if (datafile_name == NULL)
	{
		printf("Couldn't open file %s\n", datafile_name);
		exit(EXIT_FAILURE); //to quit the entire program
	}

	return datafile;
}

// prints out pc (default orig), ir (default 0), cc (default Z), and running flag (default 1)
void dump_CPU(CPU *cpu) {
	printf("PC: x%.4hx\tIR: x%.4x\tCC: %c\t     RUNNING: %d", (*cpu).pc, (*cpu).ir, (*cpu).cc, (*cpu).running);

	dump_registers(cpu);
}

// prints memory info for any memory locations not containing a zero
void dump_memory(CPU *cpu) {
	Word temp_loc = (*cpu).orig;
	
	printf("\nMEMORY (non-zero values from x0000 - xFFFF):\n");
	for (int i = (*cpu).orig; i < MEMLEN; i++)
	{
		if ((*cpu).mem[i] != 0)
		{
			printf("x%.4hX: ", temp_loc);
			
			Address temp_mem = (*cpu).mem[i];
			signed short next_loc = temp_loc + 1;
			
			//flag variables for signed or unsigned values
			cmd imm5Flag;
			cmd jsrFlag;
			cmd brFlag;
			
			//masks to save flag bit
			imm5Flag = temp_mem & 0x0020;
			jsrFlag = temp_mem & 0x0800;
			brFlag = temp_mem & 0x0E00;
			
			//variables for parts of the bitstring
			cmd opCode;
			cmd dst;
			cmd src1;
			cmd src2;
			cmd base;
			cmd trap;
			
			//shifts to remove excess bits
			cmd shift4 = (temp_mem << 4); 
			cmd shift7 = (temp_mem << 7);
			cmd shift8 = (temp_mem << 8);
			cmd shift13 = (temp_mem << 13);
			
			//shifts to set value according to shifts
			opCode = (temp_mem >> 12);
			dst = (shift4 >> 13);
			src1 = (shift7 >> 13);
			src2 = (shift13 >> 13);
			base = (shift7 >> 13);
			trap = (shift8 >> 8);
			
			//variables for signed parts of the bitstring (where +- is important)
			signed short imm5;
			signed short PCoffset9;
			signed short PCoffset11;
			signed short LDRoffset;
			
			//shifts to remove excess bits
			signed short sShift5 = (temp_mem << 5);
			signed short sShift7 = (temp_mem << 7);
			signed short sShift10 = (temp_mem << 10);
			signed short sShift11 = (temp_mem << 11);
			
			//shifts to set value according to shifts
			imm5 = (sShift11 >> 11);
			PCoffset9 = (sShift7 >> 7);
			PCoffset11 = (sShift5 >> 5);
			LDRoffset = (sShift10 >> 10);
			
			//output according to opcode
			switch ((opCode))
			{
				case  0:
					PCoffset9 += next_loc;
					if (brFlag == 0x0800)
					{
						printf("x%.4hX\t% d\tBRN\tx%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], PCoffset9);
					}
					else if (brFlag == 0x0400)
					{
						printf("x%.4hX\t% d\tBRZ\tx%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], PCoffset9);
					}
					else if (brFlag == 0x0200)
					{
						printf("x%.4hX\t% d\tBRP\tx%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], PCoffset9);
					}
					else if (brFlag == 0x0C00)
					{
						printf("x%.4hX\t% d\tBRNZ\tx%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], PCoffset9);
					}
					else if (brFlag == 0x0A00)
					{
						printf("x%.4hX\t% d\tBRNP\tx%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], PCoffset9);
					}
					else if (brFlag == 0x0600)
					{
						printf("x%.4hX\t% d\tBRZP\tx%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], PCoffset9);
					}
					else if (brFlag == 0x0E00)
					{
						printf("x%.4hX\t% d\tBRNZP\tx%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], PCoffset9);
					}
					else
					{
						printf("x%.4hX\t% d\tNOP\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					break;
				case 1:
					if (imm5Flag == 0x0020)
					{
						printf("x%.4hX\t% d\tADD\tR%d, R%d, #%d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, imm5);
					}
					else if(imm5Flag == 0x0000)
					{
						printf("x%.4hX\t% d\tADD\tR%d, R%d, R%d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, src2);
					}
						
					break;
				case 2:
					PCoffset9 += next_loc;
					printf("x%.4hX\t% d\tLD\tR%d, x%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], dst, PCoffset9);
					break;
				case 3:
					PCoffset9 += next_loc;
					//dst is same as src for ST
					printf("x%.4hX\t% d\tST\tR%d, x%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], dst, PCoffset9);
					break;
				case 4:
					if (jsrFlag == 0x0800)
					{
						PCoffset11 += next_loc;
						printf("x%.4hX\t% d\tJSR\tx%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], PCoffset11);
					}
					else if(jsrFlag == 0x0000)
					{
						printf("x%.4hX\t% d\tJSRR\tR%d\n", (*cpu).mem[i], (*cpu).mem[i], base);
					}
					break;
				case 5:
					if (imm5Flag == 0x0020)
					{
						printf("x%.4hX\t% d\tAND\tR%d, R%d, #%d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, imm5);
					}
					else if(imm5Flag == 0x0000)
					{
						printf("x%.4hX\t% d\tAND\tR%d, R%d, R%d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, src2);
					}
					break;
				case 6:
					printf("x%.4hX\t% d\tLDR\tR%d, R%d, #%d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, LDRoffset);
					break;
				case 7:
					//dst is same as src for STR
					printf("x%.4hX\t% d\tSTR\tR%d, R%d, #%d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, LDRoffset);
					break;
				case 8:
					printf("x%.4hX\t% d\tRTI\n", (*cpu).mem[i], (*cpu).mem[i]);
					break;
				case 9:
					printf("x%.4hX\t% d\tNOT\tR%d, R%d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1);
					break;
				case 10:
					PCoffset9 += next_loc;
					printf("x%.4hX\t% d\tLDI\tR%d, x%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], dst, PCoffset9);
					break;
				case 11:
					PCoffset9 += next_loc;
					//dst is same as src for STI
					printf("x%.4hX\t% d\tSTI\tR%d, x%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], dst, PCoffset9);
					break;
				case 12:
					//src1 same as base for JMP
					if (src1 == 7)
					{
						printf("x%.4hX\t% d\tRET\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					else
					{
						printf("x%.4hX\t% d\tJMP\tR%d\n", (*cpu).mem[i], (*cpu).mem[i], src1);
					}
					break;
				case 13:
					printf("x%.4hX\t% d\terr\n", (*cpu).mem[i], (*cpu).mem[i]);
					break;
				case 14:
					PCoffset9 += next_loc;
					printf("x%.4hX\t% d\tLEA\tR%d, x%.4hX\n", (*cpu).mem[i], (*cpu).mem[i], dst, PCoffset9);
					break;
				case 15:
					if (trap == 0x20)
					{					
						printf("x%.4hX\t% d\tTRAP\tGETC\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					else if (trap == 0x21)
					{
						printf("x%.4hX\t% d\tTRAP\tOUT\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					else if (trap == 0x22)
					{
						printf("x%.4hX\t% d\tTRAP\tPUTS\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					else if (trap == 0x23)
					{
						printf("x%.4hX\t% d\tTRAP\tIN\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					else if (trap == 0x25)
					{
						printf("x%.4hX\t% d\tTRAP\tHALT\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					else
					{
						printf("x%.4hX\t% d\tTRAP\tx%.2hX\n", (*cpu).mem[i], (*cpu).mem[i], trap);
					}
					break;
				default:
					printf("BAD OPCODE: %d; quitting\n", opCode);
					(*cpu).running = 0;
					break;
			}			
		}
		temp_loc++;
	}
}

//Print all 8 register values
void dump_registers(CPU *cpu) {
	// *** STUB ****
	printf("\n     R0: x%.4hx   R1: x%.4hx   R2: x%.4hx   R3: x%.4hx", (*cpu).reg[0], (*cpu).reg[1], (*cpu).reg[2], (*cpu).reg[3]);
	printf("\n     R4: x%.4hx   R5: x%.4hx   R6: x%.4hx   R7: x%.4hx", (*cpu).reg[4], (*cpu).reg[5], (*cpu).reg[6], (*cpu).reg[7]);
}
