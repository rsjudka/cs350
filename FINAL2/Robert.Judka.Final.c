/*EXTRA CREDIT DID:
	1. VERY VERY VERY FANCY FANCY OUTPUT
		- memory dump does the loopty loop to x0000 when xFFFF is reached!
		- OCD like spacing within the output
		- added the RET function (similar as JMP, but with some additional goodies) as used in the lc-3 simulator
		- nifty suprise when calling for help ;)
	2. ALL THE *EXTRAZZZZ
		- dumping a specific section of memory
		- changing the value of a memory location
		- changing the value of a register
		- changing the PC
		- changing the running flag back to 1 after a HALT sets it to 0
		- changing the condition code
		*Dr. Jim Sasaki allowed the assumption the user is smart enough to only enter hex values
*/

//Robert Judka
// CS 350, Spring 2016
// Final Project, part 2

#include <stdio.h>
#include <stdlib.h>			// For error exit()
#include <string.h>			//used to store data from .hex file
#include <stddef.h>			//used to decode hex value from .hex file

	typedef unsigned short int Word;
	typedef unsigned short int Address;
	typedef unsigned short cmd;

	#define MEMLEN 65536
	#define NREG 8

	typedef struct {
		signed short int mem[MEMLEN];
		Word orig;
		signed short int reg[NREG];		//array holding registers 1-8
		Address pc;						//Program Counter
		int running;					//running = 1 iff CPU is executing instructions
		Word ir;						//Instruction Register
		char cc;						//Conditional Code
  		int opcode;						//Operation to execute
		int reg_R;						//Registry field
		int addr_MM;					//Memory field
		Word memLoop;					//value to loop if memory locations get full, allows for a loop back to mem location 0
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
	void dump_memory(CPU *cpu, Word start, int single); //single used to return if a single dump is called
	void dump_registers(CPU *cpu);

//Main program: Initialize the cpu, and read the initial memory values
int main(int argc, char *argv[]) {
	printf("Robert Judka CS350-01\n");
	printf("LC-3 Simulator: Final Project pt. 2\n");
	CPU cpu_value, *cpu = &cpu_value;
	initialize_CPU(cpu);
	initialize_memory(argc, argv, cpu);
	printf("\nINITIAL CPU:\n");
	dump_CPU(cpu);
	printf("\n");
	printf("\nMEMORY (non-zero values from x0000 - xFFFF):\n");
	dump_memory(cpu, (*cpu).orig, 0);

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
    char *read_success;	// NULL if reading in a line fails.

    int nbr_cycles;     // Number of instruction cycles to execute
    char cmd_char;      // Command 'q', 'h', '?', 'd', or '\n'
    int done = 0;       // Should simulator stop?

	int value_read, words_read, loc = 0;

    read_success = fgets(cmd_line, CMD_LINE_LEN, stdin);

	////used to distinguish between char, int, and EOF
	value_read = cmd_line[loc];
	words_read = sscanf(cmd_line, "%d", &value_read);
  	//Quit if read_success indicates end-of-file
	if (read_success == NULL)
	{
		done = 1;
	}

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
	#define BUFFERSIZE 100
    if (cmd_char == '?' || cmd_char == 'h')
	{
        help_message();
    }
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
		printf("\nMEMORY (non-zero values from x0000 - xFFFF):\n");
		dump_memory(cpu, 0, 0);
		printf("\n");
	}
	else if (cmd_char == '\n')
	{
		one_instruction_cycle(cpu);
	}
	else if (cmd_char == 'a')
	{
		char userCC[BUFFERSIZE]; //holds the first char of user input
		char garbage; //used to hold remaining chars from user input
		printf("Enter the value of the new CC: ");
		scanf("%s", userCC);
		if ((userCC[0] == 'Z') || (userCC[0] == 'P') || (userCC[0] == 'N'))
		{
			(*cpu).cc = userCC[0];
			printf("new CC: %c\n", userCC[0]);
		}
		else
		{
			printf("Not a valid CC. No change made\n");
		}
		scanf("%c", &garbage);
	}
	else if (cmd_char == 'c')
	{
		int userMem;
		printf("Enter the memory location you want to change: ");;
		scanf("%x", &userMem);
		while (getchar() != '\n'); //skips chars until user hits return
		if ((userMem >= 0) || (userMem <= MEMLEN))
		{
			unsigned int userValue;
			printf("Enter the new hex value: ");
			scanf("%x", &userValue);
			(*cpu).mem[userMem] = userValue;
			printf("M[x%.4hX] = x%.4hX\n", userMem, (*cpu).mem[userMem]);
		}
		else
		{
			printf("Not a valid memory location. No change made\n");
		}
		while (getchar() != '\n');
	}
	else if (cmd_char == 'f')
	{
		(*cpu).running = 1;
		printf("running flag = 1\n");
	}
	else if (cmd_char == 'p')
	{
		unsigned int userPC;
		printf("Enter the hex value of the new PC: ");
		scanf("%x", &userPC);
		if ((userPC >= 0) || (userPC <= MEMLEN))
		{
			(*cpu).pc = userPC;
			printf("new PC: x%.4hX\n", userPC);
		}
		else
		{
			printf("Not a valid PC. No change made\n");
		}
		while (getchar() != '\n');
	}
	else if (cmd_char == 'r')
	{
		int userReg;
		printf("Enter the register you want to change: ");
		scanf("%d", &userReg);
		while (getchar() != '\n');
		if (userReg < 0 || userReg > NREG)
		{
			printf("Not a valid register. No change made\n");
		}
		else
		{
			unsigned int userValue;
			printf("Enter the new hex value: ");
			scanf("%x", &userValue);
			(*cpu).reg[userReg] = userValue;
			printf("R%d = x%.4hX\n", userReg, (*cpu).reg[userReg]);
			while (getchar() != '\n');	
		}
	}
	else if (cmd_char == 's')
	{
		unsigned int userMem;
		printf("Enter the memory location you want to dump: ");
		scanf("%x", &userMem);
		if ((userMem >= 0) || (userMem <= MEMLEN))
		{
			dump_memory(cpu, userMem, 1);
		}
		else
		{
			printf("Not a valid memory location. Nothing to dump\n");
		}
		while (getchar() != '\n');
	}
	else
	{
		printf("no such thing!\n");
	}
    return 0;
}

// Print standard message for simulator help command ('h' or '?')
void help_message(void) {
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
		}
	}
	printf("\n\nJK ain't nobody got time for dat!\n");
	printf("\nHere is a list of your options:\n");
	printf("\t'a' - change the conditional code (case sensitive)\n");
	printf("\t'c' - *change the value of a memory location\n");
	printf("\t'd' - dumps out CPU\n");
	printf("\t'f' - change running flag to 1\n");
	printf("\t'h' or '?' - help menu (seems kinda redundant at this point)\n");
	printf("\t'p' - *change the PC\n");
	printf("\t'q' - quits the program\n");
	printf("\t'r' - *change the value of a register\n");
	printf("\t's' - *dumps a specific section of memory\n");
	printf("\tan integer > 0 - executes N instruction cycles (don't get too crazy with this one!)\n");
	printf("\treturn - executes one instruction cycle\n");
	printf("\t\t*(*YOU MUST ENTER A 4-DIGITHEX VALUE OR ELSE!)\n");
	printf("\t\t\t*this assumption was approved by Dr. Jim Sasaki\n");
}

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
	//checks to make sure running flag is 1
	//if not, print halt
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
		int tempOffset = 0; //saves the orignal offset value
		Address target = 0; //target for jumps, temp location for the pc
		switch ((opCode))
		{
			/*
			Any temp variable made was to do all the op code calculations
			for some strange reason when I did the calculations directly on
			the values themselves, it would randomly change. It may get messy
			sometimes, but it works!
			*/
			case  0:
				tempOffset = PCoffset9; //used to hold original value of PCoffset
				PCoffset9 += next_loc;
				if (brFlag == 0x0800)
				{
					printf("x%.4hX\tBRN\t%d", (*cpu).mem[instr_loc], tempOffset);
					if ((*cpu).cc == 'N')
					{
						printf("\t\t; CC = %c, PC <- x%.4hX + %d = x%.4hX\n", (*cpu).cc, (*cpu).pc, tempOffset, PCoffset9);
						(*cpu).pc = PCoffset9;
					}
					else
					{
						printf("\t\t; CC = %c, no branch\n", (*cpu).cc);
					}
				}
				else if (brFlag == 0x0400)
				{
					printf("x%.4hX\tBRZ\t%d", (*cpu).mem[instr_loc], tempOffset);
					if ((*cpu).cc == 'Z')
					{
						printf("\t\t; CC = %c, PC <- x%.4hX + %d = x%.4hX\n", (*cpu).cc, (*cpu).pc, tempOffset, PCoffset9);
						(*cpu).pc = PCoffset9;
					}
					else //checks branch against condtitional code
					{
						printf("\t\t; CC = %c, no branch\n", (*cpu).cc);
					}
				}
				else if (brFlag == 0x0200)
				{
					printf("x%.4hX\tBRP\t%d", (*cpu).mem[instr_loc], tempOffset);
					if ((*cpu).cc == 'P')
					{
						printf("\t\t; CC = %c, PC <- x%.4hX + %d = x%.4hX\n", (*cpu).cc, (*cpu).pc, tempOffset, PCoffset9);
						(*cpu).pc = PCoffset9;
					}
					else //checks branch against condtitional code
					{
						printf("\t\t; CC = %c, no branch\n", (*cpu).cc);
					}
				}
				else if (brFlag == 0x0C00)
				{
					printf("x%.4hX\tBRNZ\t%d", (*cpu).mem[instr_loc], tempOffset);
					if (((*cpu).cc == 'N') ||((*cpu).cc == 'Z'))
					{
						printf("\t\t; CC = %c, PC <- x%.4hX + %d = x%.4hX\n", (*cpu).cc, (*cpu).pc, tempOffset, PCoffset9);
						(*cpu).pc = PCoffset9;
					}
					else //checks branch against condtitional code
					{
						printf("\t\t; CC = %c, no branch\n", (*cpu).cc);
					}
				}
				else if (brFlag == 0x0A00)
				{
					printf("x%.4hX\tBRNP\t%d", (*cpu).mem[instr_loc], tempOffset);
					if (((*cpu).cc == 'N') ||((*cpu).cc == 'P'))
					{
						printf("\t\t; CC = %c, PC <- x%.4hX + %d = x%.4hX\n", (*cpu).cc, (*cpu).pc, tempOffset, PCoffset9);
						(*cpu).pc = PCoffset9;
					}
					else //checks branch against condtitional code
					{
						printf("\t\t; CC = %c, no branch\n", (*cpu).cc);
					}
				}
				else if (brFlag == 0x0600)
				{
					printf("x%.4hX\tBRZP\t%d", (*cpu).mem[instr_loc], tempOffset);
					if (((*cpu).cc == 'Z') ||((*cpu).cc == 'P'))
					{
						printf("\t\t; CC = %c, PC <- x%.4hX + %d = x%.4hX\n", (*cpu).cc, (*cpu).pc, tempOffset, PCoffset9);
						(*cpu).pc = PCoffset9;
					}
					else //checks branch against condtitional code
					{
						printf("\t\t; CC = %c, no branch\n", (*cpu).cc);
					}
				}
				else if (brFlag == 0x0E00)
				{
					printf("x%.4hX\tBRNZP\t%d", (*cpu).mem[instr_loc], tempOffset);
					printf("\t\t; CC = %c, PC <- x%.4hX + %d = x%.4hX\n", (*cpu).cc, (*cpu).pc, tempOffset, PCoffset9);
					(*cpu).pc = PCoffset9;
				}
				else //checks branch against condtitional code
				{
					printf("x%.4hX\t% d\tNOP", (*cpu).mem[instr_loc], (*cpu).mem[instr_loc]);
					printf("\t\t; CC = %c, no branch\n", (*cpu).cc);
				}
				break;
			case 1:
				if (imm5Flag == 0x0020)
				{
					printf("x%.4hX\tADD\tR%d, R%d, %d", (*cpu).mem[instr_loc], dst, src1, imm5);
					signed short int tempReg = (*cpu).reg[src1];
					(*cpu).reg[dst] = (*cpu).reg[src1] + imm5;
					
					if ((*cpu).reg[dst] == 0)
					{
						(*cpu).cc = 'Z';
					}
					else if ((*cpu).reg[dst] > 0)
					{
						(*cpu).cc = 'P';
					}
					else if ((*cpu).reg[dst] < 0)
					{
						(*cpu).cc = 'N';
					}
					printf("\t; R%d <- x%.4hX + %d = x%.4hX; CC = %c\n", dst, tempReg, imm5, (*cpu).reg[dst], (*cpu).cc);
				}
				else if(imm5Flag == 0x0000)
				{
					printf("x%.4hX\tADD\tR%d, R%d, R%d", (*cpu).mem[instr_loc], dst, src1, src2);
					signed short int tempReg = (*cpu).reg[src1];
					(*cpu).reg[dst] = (*cpu).reg[src1] + (*cpu).reg[src2];
					
					if ((*cpu).reg[dst] == 0)
					{
						(*cpu).cc = 'Z';
					}
					else if ((*cpu).reg[dst] > 0)
					{
						(*cpu).cc = 'P';
					}
					else if ((*cpu).reg[dst] < 0)
					{
						(*cpu).cc = 'N';
					}
					printf("\t; R%d <- x%.4hX + x%.4hX = x%.4hX; CC = %c\n", dst, tempReg, (*cpu).reg[src2], (*cpu).reg[dst], (*cpu).cc);
				}
				break;
			case 2:
				tempOffset = PCoffset9;
				PCoffset9 += next_loc;
				printf("x%.4hX\tLD\tR%d, %d", (*cpu).mem[instr_loc], dst, tempOffset);
				(*cpu).reg[dst] = (*cpu).mem[abs((*cpu).pc + tempOffset)];
				
				if ((*cpu).reg[dst] == 0)
				{
					(*cpu).cc = 'Z';
				}
				else if ((*cpu).reg[dst] > 0)
				{
					(*cpu).cc = 'P';
				}
				else if ((*cpu).reg[dst] < 0)
				{
					(*cpu).cc = 'N';
				}
				if ((tempOffset > -100)) //used to make a nice looking output, accounting for the size of the offset
				{
					printf("\t\t; R%d <- M[x%.4hX] = x%.4hX; CC = %c\n", dst, ((*cpu).pc + tempOffset), (*cpu).reg[dst], (*cpu).cc);
				}
				else
				{
					printf("\t; R%d <- M[x%.4hX] = x%.4hX; CC = %c\n", dst, ((*cpu).pc + tempOffset), (*cpu).reg[dst], (*cpu).cc);
				}
				break;
	 		case 3:
			 	tempOffset = PCoffset9;
				PCoffset9 += next_loc;
				//dst is same as src for ST
				printf("x%.4hX\tST\tR%d, %d", (*cpu).mem[instr_loc], dst, tempOffset);
				(*cpu).mem[abs((*cpu).pc + tempOffset)] = (*cpu).reg[dst];
				if ((tempOffset > -100)) //used to make a nice looking output, accounting for the size of the offset
				{
					printf("\t\t; M[x%.4hX] <- x%.4hX\n", ((*cpu).pc + tempOffset), (*cpu).reg[dst]);
				}
				else
				{
					printf("\t; M[x%.4hX] <- x%.4hX\n", ((*cpu).pc + tempOffset), (*cpu).reg[dst]);
				}
				break;
			case 4:
				if (jsrFlag == 0x0800)
				{
					tempOffset = PCoffset11;
					PCoffset11 += next_loc;
					printf("x%.4hX\tJSR\t%d", (*cpu).mem[instr_loc], tempOffset);
					(*cpu).reg[7] = (*cpu).pc;
					(*cpu).pc = PCoffset11;
					printf("\t\t; PC <- x%.4hX + %d = x%.4hX, R7 <- x%.4hX\n", (*cpu).reg[7], tempOffset, (*cpu).pc, (*cpu).reg[7]);
				}
				else if(jsrFlag == 0x0000)
				{
					printf("x%.4hX\tJSRR\tR%d", (*cpu).mem[instr_loc], base);
					target = (*cpu).reg[base];
					(*cpu).reg[7] = (*cpu).pc;
					(*cpu).pc = target;
					printf("\t\t; PC <- x%.4hX, R7 <- x%.4hX\n", target, (*cpu).reg[7]);
				}
				break;
			case 5:
				if (imm5Flag == 0x0020)
				{
					printf("x%.4hX\tAND\tR%d, R%d, %d", (*cpu).mem[instr_loc], dst, src1, imm5);
					signed short int tempReg = (*cpu).reg[src1];
					(*cpu).reg[dst] = (*cpu).reg[src1] & imm5;
					
					if ((*cpu).reg[dst] == 0)
					{
						(*cpu).cc = 'Z';
					}
					else if ((*cpu).reg[dst] > 0)
					{
						(*cpu).cc = 'P';
					}
					else if ((*cpu).reg[dst] < 0)
					{
						(*cpu).cc = 'N';
					}
					printf("\t; R%d <- x%.4hX & x%.4hX = x%.4hX; CC = %c\n", dst, tempReg, imm5, (*cpu).reg[dst], (*cpu).cc);
				}
				else if(imm5Flag == 0x0000)
				{
					printf("x%.4hX\tAND\tR%d, R%d, R%d", (*cpu).mem[instr_loc], dst, src1, src2);
					(*cpu).reg[dst] = (*cpu).reg[src1] & (*cpu).reg[src2];
					
					if ((*cpu).reg[dst] == 0)
					{
						(*cpu).cc = 'Z';
					}
					else if ((*cpu).reg[dst] > 0)
					{
						(*cpu).cc = 'P';
					}
					else if ((*cpu).reg[dst] < 0)
					{
						(*cpu).cc = 'N';
					}
					printf("\t; x%.4hX & x%.4hX = x%.4hX; CC = %c\n", (*cpu).reg[src1], (*cpu).reg[src2], (*cpu).reg[dst], (*cpu).cc);
				}
				break;
			case 6:
				printf("x%.4hX\tLDR\tR%d, R%d, %d", (*cpu).mem[instr_loc], dst, src1, LDRoffset);
				unsigned short int tem = (*cpu).reg[src1] + (unsigned)LDRoffset;
				(*cpu).reg[dst] = (*cpu).mem[tem];
				
				if ((*cpu).reg[dst] == 0)
				{
					(*cpu).cc = 'Z';
				}
				else if ((*cpu).reg[dst] > 0)
				{
					(*cpu).cc = 'P';
				}
				else if ((*cpu).reg[dst] < 0)
				{
					(*cpu).cc = 'N';
				}
				printf("\t; R%d <- M[x%.4hX + %d] = M[x%.4hX] = x%.4hX; CC = %c\n", dst, (*cpu).reg[src1], LDRoffset, ((*cpu).reg[src1] + LDRoffset), (*cpu).reg[dst], (*cpu).cc);
				break;
			case 7:
				//dst is same as src for STR
				printf("x%.4hX\tSTR\tR%d, R%d, %d", (*cpu).mem[instr_loc], dst, src1, LDRoffset);
				unsigned short int temp = (*cpu).reg[src1] + (unsigned)LDRoffset;
				(*cpu).mem[temp] = (*cpu).reg[dst];
				printf("\t; M[x%.4hX + %d] = M[x%.4hX] <- x%.4hX\n", (*cpu).reg[src1], LDRoffset, temp, (*cpu).reg[dst]);
				break;
			case 8:
				printf("x%.4hX\tRTI", (*cpu).mem[instr_loc]);
				printf("\t\t\t; Opcode ignored\n");
				break;
			case 9:
				printf("x%.4hX\tNOT\tR%d, R%d", (*cpu).mem[instr_loc], dst, src1);
				signed short int tempReg = (*cpu).reg[src1];
				signed short int notReg = (-1 * tempReg) - 1;
				
				if (notReg == 0)
				{
					(*cpu).cc = 'Z';
				}
				else if (notReg > 0)
				{
					(*cpu).cc = 'P';
				}
				else if (notReg < 0)
				{
					(*cpu).cc = 'N';
				}
				printf("\t\t; R%d <- NOT x%.4hX = x%.4hX; CC = %c\n", dst, tempReg, notReg, (*cpu).cc);
				//saves the changes of the values so all the output could display nicely
				(*cpu).reg[src1] = tempReg;
				(*cpu).reg[dst] = notReg;
				break;
			case 10:
				tempOffset = PCoffset9;
				PCoffset9 += next_loc;
				printf("x%.4hX\tLDI\tR%d, %d", (*cpu).mem[instr_loc], dst, tempOffset);
				unsigned short int tempVal = (*cpu).mem[abs(tempOffset + (*cpu).pc)];
				signed short int tempR = (*cpu).mem[tempVal];
				
				if (tempR == 0)
				{
					(*cpu).cc = 'Z';
				}
				else if (tempR > 0)
				{
					(*cpu).cc = 'P';
				}
				else if (tempR < 0)
				{
					(*cpu).cc = 'N';
				}
				if ((tempOffset > -100)) //used to make a nice looking output, accounting for the size of the offset
				{
					printf("\t\t; R%d <- M[M[x%.4hX]] = M[x%.4hX] = x%.4hX; CC = %c\n", dst, (tempOffset + (*cpu).pc), tempVal, tempR, (*cpu).cc);
				}
				else 
				{
					printf("\t; R%d <- M[M[x%.4hX]] = M[x%.4hX] = x%.4hX; CC = %c\n", dst, (tempOffset + (*cpu).pc), tempVal, tempR, (*cpu).cc);
				}
				(*cpu).reg[dst] = tempR;
				break;
			case 11:
				tempOffset = PCoffset9;
				PCoffset9 += next_loc;
				//dst is same as src for STI
				printf("x%.4hX\tSTI\tR%d, %d", (*cpu).mem[instr_loc], dst, tempOffset);
				unsigned short int tempLoc1 = (*cpu).pc + (unsigned)tempOffset;
				unsigned short int tempLoc2 = (*cpu).mem[tempLoc1];
				(*cpu).mem[tempLoc2] = (*cpu).reg[dst];
				
				if ((tempOffset > -100)) //used to make a nice looking output, accounting for the size of the offset
				{
					printf("\t\t; M[M[x%.4hX]] = M[x%.4hX] <- x%.4hX\n", (tempOffset + (*cpu).pc), ((*cpu).mem[abs(tempOffset + (*cpu).pc)]), (*cpu).reg[dst]);
				}
				else 
				{
					printf("\t; M[M[x%.4hX]] = M[x%.4hX] <- x%.4hX\n", (tempOffset + (*cpu).pc), ((*cpu).mem[abs(tempOffset + (*cpu).pc)]), (*cpu).reg[dst]);
				}
				break;
			case 12:
				//src1 same as base for JMP
				if (src1 == 7)
				{
					printf("x%.4hX\tRET", (*cpu).mem[instr_loc]);
					(*cpu).pc = (*cpu).reg[src1];
					printf("\t\t\t; PC <- x%X\n", (*cpu).pc);
				}
				else
				{
					printf("x%.4hX\tJMP\tR%d", (*cpu).mem[instr_loc], src1);
					(*cpu).pc = (*cpu).reg[src1];
					printf("\t\t; PC <- x%X\n", (*cpu).pc);
				}
				break;
			case 13:
				printf("x%.4hX\terr", (*cpu).mem[instr_loc]);
				printf("\t\t\t; Reserved opcode; ignored\n");
				break;
			case 14:
				tempOffset = PCoffset9;
				PCoffset9 += next_loc;
				printf("x%.4hX\tLEA\tR%d, %d", (*cpu).mem[instr_loc], dst, tempOffset);
				(*cpu).reg[dst] = PCoffset9;
				
				if (PCoffset9 == 0)
				{
					(*cpu).cc = 'Z';
				}
				else if (PCoffset9 > 0)
				{
					(*cpu).cc = 'P';
				}
				else if (PCoffset9 < 0)
				{
					(*cpu).cc = 'N';
				}
				if ((tempOffset > -100)) //used to make a nice looking output, accounting for the size of the offset
				{
					printf("\t\t; R%d <- x%.4hX; CC = %c\n", dst, (*cpu).reg[dst], (*cpu).cc);
				}
				else 
				{
					printf("\t; R%d <- x%.4hX; CC = %c\n", dst, (*cpu).reg[dst], (*cpu).cc);
				}
				break;
			case 15:
				(*cpu).reg[7] = (*cpu).pc;
				if (trap == 0x20)
				{
					char input;
					(*cpu).cc = 'N';
					printf("x%.4hX\tTRAP\tx20", (*cpu).mem[instr_loc]);
					printf("\t\t; CC = %c; GETC: ", (*cpu).cc);
					input = read_char();
					(*cpu).reg[0] = input;
					printf("Read %c = %d\n", input, input);
				}
				else if (trap == 0x21)
				{
					(*cpu).cc = 'N';
					printf("x%.4hX\tTRAP\tx21", (*cpu).mem[instr_loc]);
					printf("\t\t; CC = %c; OUT: %d = %c\n", (*cpu).cc, (*cpu).reg[0], (*cpu).reg[0]);
				}
				else if (trap == 0x22)
				{
					(*cpu).cc = 'N';
					printf("x%.4hX\tTRAP\tx22", (*cpu).mem[instr_loc]);
					printf("\t\t; CC = %c; PUTS: ", (*cpu).cc);
					unsigned short int startString = (*cpu).reg[0];
					unsigned short int startLoc;
					do
					{
						startLoc = (*cpu).mem[startString++];
						printf("%c", startLoc);
					}
					while (startLoc != '\0');
					printf("\n");
				}
				else if (trap == 0x23)
				{
					(*cpu).cc = 'N';
					char input;
					printf("x%.4hX\tTRAP\tx23", (*cpu).mem[instr_loc]);
					printf("\t\t; CC = %c; IN: Input a character>", (*cpu).cc);
					input = read_char();
					(*cpu).reg[0] = input;
					printf("Read %c = %d\n", input, input);
				}
				else if (trap == 0x25)
				{
					printf("x%.4hX\tTRAP\tx25", (*cpu).mem[instr_loc]);
					(*cpu).running = 0;
					(*cpu).cc = 'P';
					printf("\t\t; CC = %c; Halting\n", (*cpu).cc);
				}
				else
				{
					printf("x%.4hX\tTRAP\tx%.2hX", (*cpu).mem[instr_loc], trap);
					printf("\t\t; Bad trap vector; Halting\n");
					(*cpu).running = 0;
				}
				break;
			default:
				printf("BAD OPCODE: %d; quitting\n", opCode);
				(*cpu).running = 0;
				break;
		}
    }

//reads a single character for the traps
int read_char()
{
	char buffer[3] = "";
	fgets(buffer, sizeof buffer, stdin);
	return buffer[0];
}

// Execute the halt instruction (make CPU stop running)
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
	cpu -> running = 1; //1 means running, 0 means not running
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
		strncpy(hexCode, buffer, 4); //saves the first for characters of a string into an array
		hexCode[5] = '\0';
		int toInt = 0;
		toInt = strtoul(hexCode, &p, 16); //reads the first 16 bites in an array and saves them as a int
		//Saves first hex value as the origin
		if (saveOrig == 0)
		{ 
			(*cpu).orig = toInt;
			(*cpu).pc = (*cpu).orig;
			loc = (*cpu).orig;
			addFlag = 1;
			saveOrig = 1;
			(*cpu).memLoop = 0;
		}
		else //saves all other hex values to mem[loc]
		{
			(*cpu).mem[loc] = toInt;
			addFlag = 0; 
		}
		
		//loops to mem location 0 if at last space in mem location
		if (loc == (MEMLEN-1))
		{
			loc = 0;
			addFlag = 1;
			(*cpu).memLoop = 1;
		}

		//increments location if not the origin
		if (addFlag == 0)
		{
			loc++;
		}
		read_success = fgets(buffer, sizeof(buffer), datafile);
	}
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
	printf("PC: x%.4hX\tIR: x%.4X\tCC: %c\t     RUNNING: %d", (*cpu).pc, (*cpu).ir, (*cpu).cc, (*cpu).running);

	dump_registers(cpu);
}

// prints memory info for any memory locations not containing a zero
void dump_memory(CPU *cpu, Word start, int single) {
	Word temp_loc = start;

	Word og = start; //used to specify the origin, or a single memory location to dump
	int tempMax = MEMLEN; //sets the new max to the origin when xFFFF loops to x0000, stops from duplicate dumps
	for (int i = og; i < MEMLEN; i++)
	{
		//will only print if memory location has a value
		//or if it is being called from the dump single memory location command
		if ((((*cpu).mem[i] != 0) && (tempMax > i)) || (single == 1))
		{
			//printf("\n\n%d = %x\n\n", i, (*cpu).mem[i]);
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
			int tempOffsett = 0; //used to save the offset value
			switch ((opCode))
			{
				case  0:
					tempOffsett = PCoffset9;
					PCoffset9 += next_loc;
					if (brFlag == 0x0800)
					{
						printf("x%.4hX\t% d\tBRN\t%d\n", (*cpu).mem[i], (*cpu).mem[i], tempOffsett);
					}
					else if (brFlag == 0x0400)
					{
						printf("x%.4hX\t% d\tBRZ\t%d\n", (*cpu).mem[i], (*cpu).mem[i], tempOffsett);
					}
					else if (brFlag == 0x0200)
					{
						printf("x%.4hX\t% d\tBRP\t%d\n", (*cpu).mem[i], (*cpu).mem[i], tempOffsett);
					}
					else if (brFlag == 0x0C00)
					{
						printf("x%.4hX\t% d\tBRNZ\t%d\n", (*cpu).mem[i], (*cpu).mem[i], tempOffsett);
					}
					else if (brFlag == 0x0A00)
					{
						printf("x%.4hX\t% d\tBRNP\t%d\n", (*cpu).mem[i], (*cpu).mem[i], tempOffsett);
					}
					else if (brFlag == 0x0600)
					{
						printf("x%.4hX\t% d\tBRZP\t%d\n", (*cpu).mem[i], (*cpu).mem[i], tempOffsett);
					}
					else if (brFlag == 0x0E00)
					{
						printf("x%.4hX\t% d\tBRNZP\t%d\n", (*cpu).mem[i], (*cpu).mem[i], tempOffsett);
					}
					else
					{
						printf("x%.4hX\t% d\tNOP", (*cpu).mem[i], (*cpu).mem[i]);
						printf("\t%d\n", (*cpu).mem[i]);
					}
					break;
				case 1:
					if (imm5Flag == 0x0020)
					{
						printf("x%.4hX\t% d\tADD\tR%d, R%d, %d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, imm5);
					}
					else if(imm5Flag == 0x0000)
					{
						printf("x%.4hX\t% d\tADD\tR%d, R%d, R%d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, src2);
					}

					break;
				case 2:
					tempOffsett = PCoffset9;
					PCoffset9 += next_loc;
					printf("x%.4hX\t% d\tLD\tR%d, %d\n", (*cpu).mem[i], (*cpu).mem[i], dst, tempOffsett);
					break;
				case 3:
					tempOffsett = PCoffset9;
					PCoffset9 += next_loc;
					//dst is same as src for ST
					printf("x%.4hX\t% d\tST\tR%d, %d\n", (*cpu).mem[i], (*cpu).mem[i], dst, tempOffsett);
					break;
				case 4:
					if (jsrFlag == 0x0800)
					{
						tempOffsett = PCoffset11;
						PCoffset11 += next_loc;
						printf("x%.4hX\t% d\tJSR\t%d\n", (*cpu).mem[i], (*cpu).mem[i], tempOffsett);
					}
					else if(jsrFlag == 0x0000)
					{
						printf("x%.4hX\t% d\tJSRR\tR%d\n", (*cpu).mem[i], (*cpu).mem[i], base);
					}
					break;
				case 5:
					if (imm5Flag == 0x0020)
					{
						printf("x%.4hX\t% d\tAND\tR%d, R%d, %d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, imm5);
					}
					else if(imm5Flag == 0x0000)
					{
						printf("x%.4hX\t% d\tAND\tR%d, R%d, R%d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, src2);
					}
					break;
				case 6:
					printf("x%.4hX\t% d\tLDR\tR%d, R%d, %d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, LDRoffset);
					break;
				case 7:
					//dst is same as src for STR
					printf("x%.4hX\t% d\tSTR\tR%d, R%d, %d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1, LDRoffset);
					break;
				case 8:
					printf("x%.4hX\t% d\tRTI\n", (*cpu).mem[i], (*cpu).mem[i]);
					break;
				case 9:
					printf("x%.4hX\t% d\tNOT\tR%d, R%d\n", (*cpu).mem[i], (*cpu).mem[i], dst, src1);
					break;
				case 10:
					tempOffsett = PCoffset9;
					PCoffset9 += next_loc;
					printf("x%.4hX\t% d\tLDI\tR%d, %d\n", (*cpu).mem[i], (*cpu).mem[i], dst, tempOffsett);
					break;
				case 11:
					tempOffsett = PCoffset9;
					PCoffset9 += next_loc;
					//dst is same as src for STI
					printf("x%.4hX\t% d\tSTI\tR%d, %d\n", (*cpu).mem[i], (*cpu).mem[i], dst, tempOffsett);
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
					tempOffsett = PCoffset9;
					PCoffset9 += next_loc;
					printf("x%.4hX\t% d\tLEA\tR%d, %d\n", (*cpu).mem[i], (*cpu).mem[i], dst, tempOffsett);
					break;
				case 15:
					if (trap == 0x20)
					{
						printf("x%.4hX\t% d\tTRAP\tx20\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					else if (trap == 0x21)
					{
						printf("x%.4hX\t% d\tTRAP\tx21\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					else if (trap == 0x22)
					{
						printf("x%.4hX\t% d\tTRAP\tx22\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					else if (trap == 0x23)
					{
						printf("x%.4hX\t% d\tTRAP\tx23\n", (*cpu).mem[i], (*cpu).mem[i]);
					}
					else if (trap == 0x25)
					{
						printf("x%.4hX\t% d\tTRAP\tx25\n", (*cpu).mem[i], (*cpu).mem[i]);
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
		//nice little feature that allows to print memory locations lower than the origin
		if (((*cpu).memLoop == 1) && (i == MEMLEN - 1))
		{
			tempMax = start - 1;
			i = -1;
			(*cpu).memLoop = 2;
			temp_loc = 0;
		}
		
		//used simply for the dump signel memory location extra credit
		if (single == 1)
		{
			break;
		}
	}
	(*cpu).memLoop = 1;
}

//Print all 8 register values
void dump_registers(CPU *cpu) {
	printf("\nR0: x%.4hX % .5d\tR1: x%.4hX % .5d\tR2: x%.4hX % .5d\tR3: x%.4hX % .5d", (*cpu).reg[0], (*cpu).reg[0], (*cpu).reg[1], (*cpu).reg[1], (*cpu).reg[2], (*cpu).reg[2], (*cpu).reg[3], (*cpu).reg[3]);
	printf("\nR4: x%.4hX % .5d\tR5: x%.4hX % .5d\tR6: x%.4hX % .5d\tR7: x%.4hX % .5d", (*cpu).reg[4], (*cpu).reg[4], (*cpu).reg[5], (*cpu).reg[5], (*cpu).reg[6], (*cpu).reg[6], (*cpu).reg[7], (*cpu).reg[7]);
}