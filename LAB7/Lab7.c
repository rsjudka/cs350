//Robert Judka
// CS 350, Spring 2016
// Lab 7: SDC Simulator, part 2

#include <stdio.h>
#include <stdlib.h>       // For error exit()

// CPU Declarations -- a CPU is a structure with fields for the
// different parts of the CPU.
//
    typedef short int Word;          // type that represents a word of SDC memory
    typedef unsigned char Address;   // type that represents an SDC address

    #define MEMLEN 100
    #define NREG 10

    typedef struct {
        Word mem[MEMLEN];
        Word reg[NREG];      // Note: "register" is a reserved word
        Address pc;          // Program Counter
        int running;         // running = 1 iff CPU is executing instructions
        Word ir;             // Instruction Register
        int instr_sign;      //   sign of instruction
        int opcode;          //   opcode field
        int reg_R;           //   register field
        int addr_MM;         //   memory field
    } CPU;


// Prototypes [note the functions are also declared in this order]
//
    // *** STUB *** Lab 6 items omitted ....
	int main(int argc, char *argv[]);
    int read_execute_command(CPU *cpu);
    int execute_command(char cmd_char, CPU *cpu);
    void help_message(void);
    void many_instruction_cycles(int nbr_cycles, CPU *cpu);
    void one_instruction_cycle(CPU *cpu);

    void exec_HLT(CPU *cpu);
	void exec_INSTR(CPU *cpu);
	
	void initialize_CPU(CPU *cpu);
	void initialize_memory(int argc, char *argv[], CPU *cpu);
	FILE *get_datafile(int argc, char *argv[]);

	void dump_CPU(CPU *cpu);
	void dump_memory(CPU *cpu);
	void dump_registers(CPU *cpu);
    // *** STUB ***

// Main program: Initialize the cpu, and read the initial memory values
//
int main(int argc, char *argv[]) {
    printf("Robert Judka CS350-01\n");
	printf("SDC Simulator: Lab 7\n");
    CPU cpu_value, *cpu = &cpu_value;

// *** STUB *** Lab 6 items omitted (initialize CPU and memory) ....
	initialize_CPU(cpu);
	initialize_memory(argc, argv, cpu);

    char *prompt = "> ";
    printf("\n\nWaiting on your call mate!\n\tType 'h' or '?' for help\n%s", prompt);

    int done = read_execute_command(cpu);
    while (!done) {
        printf("%s", prompt);
        done = read_execute_command(cpu);
    }
    return 0;
}


// *** STUB ***
// Lab 6 items omitted ....
//    initialize_CPU, ..., dump registers


// Read a simulator command from the keyboard (q, h, ?, d, number,
// or empty line) and execute it.  Return true if we hit end-of-input
// or execute_command told us to quit.  Otherwise return false.
//
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
		dump_memory(cpu);
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
	
	printf("\nSDC Instruction Set:");
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
	printf("Waiting on your call mate!\n");

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
	if ((*cpu).running == 0)
	{
		printf("HALTED! Sorry CPU is not running, he gave up around mile 3 :/\n");
		return;
	}
	if (!((*cpu).pc >= 0 && (*cpu).pc <= 99))
	{
		printf("Sorry PC out of range! You broke him :(\n");
		return;
	}

    // Get instruction and increment pc
    //
    int instr_loc = cpu -> pc;  // Instruction's location (pc before increment)
    cpu -> ir = cpu -> mem[cpu -> pc];
    (cpu -> pc)++;

    // Decode instruction into opcode, reg_R, addr_MM, and instruction sign
    //
    // *** STUB ****
	if (abs((*cpu).ir) < 9000)
	{
		(*cpu).opcode = abs((*cpu).ir/1000);
	}
	else 
	{
		(*cpu).opcode = abs((*cpu).ir/100);
	}
	(*cpu).reg_R = abs(((*cpu).ir/100)%10);
	(*cpu).addr_MM = abs((*cpu).ir%100);
	if ((*cpu).ir < 0)
	{
		(*cpu).instr_sign = -1;
	}
	else
	{
		(*cpu).instr_sign = 1;
	}

    // Echo instruction
    // *** STUB ***
	char *instr[MEMLEN];
	instr[0] = "HALT";
	instr[1] = "LD";
	instr[2] = "ST";
	instr[3] = "ADD";
	instr[4] = "NEG";
	instr[5] = "LDM";
	instr[6] = "ADDM";
	instr[7] = "BR";
	instr[8] = "BRC";
	instr[90] = "GETC";
	instr[91] = "OUT";
	instr[92] = "PUTS";
	instr[93] = "DMP";
	instr[94] = "MEM";
	instr[95] = "NOP";
	
	int tempReg;
	char outChar;
	char cmd_line[CMD_LINE_LEN];
	char usr_char;
	char *read_success;
    switch (cpu -> opcode) {
    case 0: 
		printf("%.2d % .4d   %s\t\t |  Running flag <- 0\n", instr_loc, (*cpu).ir, instr[(*cpu).opcode]);
		exec_HLT(cpu); 
		break;
    // *** STUB ****
	case 1: 
		(*cpu).reg[(*cpu).reg_R] = (*cpu).mem[(*cpu).addr_MM];
		printf("%.2d % .4d   %s\tR%d, % .2d  |  R%d <- M[%d] = %d\n", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).reg_R, (*cpu).addr_MM, (*cpu).reg_R, (*cpu).addr_MM, (*cpu).reg[(*cpu).reg_R]);
		break;
	case 2:
	(*cpu).mem[(*cpu).addr_MM] = (*cpu).reg[(*cpu).reg_R];
	printf("%.2d % .4d   %s\tR%d, % .2d  |  M[%d] <- R%d = %d\n", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).reg_R, (*cpu).addr_MM, (*cpu).addr_MM, (*cpu).reg_R, (*cpu).mem[(*cpu).addr_MM]);
		break;
	case 3: 
		tempReg = cpu -> reg[(*cpu).reg_R];
		(*cpu).reg[(*cpu).reg_R] += (*cpu).mem[(*cpu).addr_MM];
		printf("%.2d % .4d   %s\tR%d, % .2d  |  R%d <- R%d + M[%d] = %d + %d = %d\n", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).reg_R, (*cpu).addr_MM, (*cpu).reg_R, (*cpu).reg_R, (*cpu).addr_MM, tempReg, (*cpu).mem[(*cpu).addr_MM], (*cpu).reg[(*cpu).reg_R]);
		break;
	case 4:
		(*cpu).reg[(*cpu).reg_R] *= -1;
		printf("%.2d % .4d   %s\tR%d\t |  R%d <- -R%d = %d\n", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).reg_R, (*cpu).reg_R, (*cpu).reg_R, (*cpu).reg[(*cpu).reg_R]);
		break;
	case 5: 
		(*cpu).reg[(*cpu).reg_R] = (*cpu).addr_MM*(*cpu).instr_sign;
		printf("%.2d % .4d   %s\tR%d, % .2d  |  R%d <- %d\n", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).reg_R, (*cpu).addr_MM*(*cpu).instr_sign, (*cpu).reg_R, (*cpu).reg[(*cpu).reg_R]);
		break;
	case 6:
	tempReg = (*cpu).reg[(*cpu).reg_R];
		(*cpu).reg[(*cpu).reg_R] += (*cpu).addr_MM*(*cpu).instr_sign;
		printf("%.2d % .4d   %s\tR%d, % .2d  |  R%d <- R%d + %d = %d + %d = %d\n", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).reg_R, (*cpu).addr_MM*(*cpu).instr_sign, (*cpu).reg_R, (*cpu).reg_R, (*cpu).addr_MM*(*cpu).instr_sign, tempReg, (*cpu).addr_MM*(*cpu).instr_sign, (*cpu).reg[(*cpu).reg_R]);
		break;
	case 7:
		(*cpu).pc = (*cpu).addr_MM;
		printf("%.2d % .4d   %s\t    % .2d  |  PC <- %d\n", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).addr_MM, (*cpu).pc);
		break;
	case 8:
		if ((*cpu).ir > 0)
		{
			instr[8] = "BRP";
		}
		else if ((*cpu).ir < 0)
		{
			instr[8] = "BRN";
		}
		if (((*cpu).ir > 0 && (*cpu).instr_sign == 1) || ((*cpu).ir < 0 && (*cpu).instr_sign == -1))
		{
			(*cpu).pc = (*cpu).addr_MM;
		}
		if ((*cpu).reg[(*cpu).reg_R] >= 0)
		{
			printf("%.2d % .4d   %s\tR%d, % .2d  |  PC <- %d if R%d = %d > 0: Yes\n", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).reg_R, (*cpu).addr_MM, (*cpu).pc, (*cpu).reg_R, (*cpu).reg[(*cpu).reg_R]);
		}
		else
		{
			printf("%.2d % .4d   %s\tR%d, % .2d  |  PC <- %d if R%d = %d < 0: Yes\n", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).reg_R, (*cpu).addr_MM, (*cpu).pc, (*cpu).reg_R, (*cpu).reg[(*cpu).reg_R]);
		}
		break;
	case 90: 
		#define CMD_LINE_LEN 80
		printf("%.2d % .4d   %s\t\t |  ", instr_loc, (*cpu).ir, instr[(*cpu).opcode]);
		printf("Enter a char mate (and/or press return): ");
		read_success = fgets(cmd_line, CMD_LINE_LEN, stdin);
		usr_char = *read_success;
		(*cpu).reg[0] = usr_char;
		//printf("\t\t\t |  R0 <- %d%c", (*cpu).reg[0], getchar());
		printf("\t\t\t |  R0 <- %d\n", (*cpu).reg[0]);
		break;
	case 91:
		outChar = (*cpu).reg[0];
		printf("%.2d % .4d   %s\t\t |  Print char in R%d (= %d): ", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).reg_R, outChar);
		printf("%c\n", outChar);
		break;
	case 92: 
		printf("%.2d % .4d   %s\t    % .2d  |  String: ", instr_loc, (*cpu).ir, instr[(*cpu).opcode], (*cpu).addr_MM);
		int temp = (*cpu).addr_MM;
		
		while ((*cpu).mem[temp] != 0)
		{
			printf("%c", (*cpu).mem[temp]);
			temp++;
		}
		printf("\n");
		break;
	case 93:
		printf("%.2d % .4d   %s\t\t \n", instr_loc, (*cpu).ir, instr[(*cpu).opcode]);
		printf("\n");
		dump_CPU(cpu);
		printf("\n\n");
		break;
	case 94: 
		printf("%.2d % .4d   %s\t\t \n", instr_loc, (*cpu).ir, instr[(*cpu).opcode]);
		dump_memory(cpu);
		printf("\n\n");
		break;
	case 95:
	case 96:
	case 97:
	case 98:
	case 99:
		printf("%.2d % .4d   %s\t\t \n", instr_loc, (*cpu).ir, instr[(*cpu).opcode]);
		break;
    default: printf("Bad opcode!? %d\n", cpu -> opcode);
    }
}

// Execute the halt instruction (make CPU stop running)
//
void exec_HLT(CPU *cpu) {
    cpu -> running = 0;
}

//part of Lab 6
void initialize_CPU(CPU *cpu) {
	
	// *** STUB ***
	for (int i = 0; i < NREG; i++)
	{
		cpu -> reg[i] = 0;
	}
	for (int i = 0; i < MEMLEN; i++)
	{
		cpu -> mem[i] = 0;
	}
	cpu -> ir = 0;
	cpu -> pc = 0;
	cpu -> instr_sign = 1;
	cpu -> running = 1;
	cpu -> opcode = 0;
	
	printf("\nINITIAL CPU:\n");	
	dump_CPU(cpu);
	printf("\n");
}

// Read and dump initial values for memory
//
void initialize_memory(int argc, char *argv[], CPU *cpu) {
	FILE *datafile = get_datafile(argc, argv);

// Buffer to read next line of text into
#define BUFFER_LEN 80
	char buffer[BUFFER_LEN];

	// Will read the next line (words_read = 1 if it started
	// with a memory value). Will set memory location loc to
	// value_read
	//
	int value_read, words_read, loc = 0, done = 0;

	char *read_success;    // NULL if reading in a line fails.
	
	read_success = fgets(buffer, BUFFER_LEN, datafile);
	while (read_success != NULL && !done) {
		// If the line of input begins with an integer, treat
		// it as the memory value to read in.  Ignore junk
		// after the number and ignore blank lines and lines
		// that don't begin with a number.
		//
		
		value_read = buffer[loc];
		words_read = sscanf(buffer, "%d", &value_read);
		if (words_read == 1)
		{
			
			// *** STUB *** set memory value at current location to
			// value_read and increment location.  Exceptions: If
			// loc is out of range, complain and quit the loop. If
			// value_read is outside -9999...9999, then it's a
			// sentinel and we should say so and quit the loop.
			if (loc > MEMLEN)
			{
				printf("\nLocation %d out of range\n", loc);
				done = 1;
			}
			else if ((value_read < -9999) | (value_read > 9999))
			{
				printf("\n***Sentinel %d found at location %d***\n", value_read, loc);
				done = 1;
				//cpu -> pc--;
			}
			else
			{
				cpu -> mem[loc] = value_read;
				//cpu -> pc++;
				loc++;
			}
		}
		
		// Get next line and continue the loop
		//
		// *** STUB ***
		read_success = fgets(buffer, BUFFER_LEN, datafile);
		words_read = sscanf(buffer, "%d", &value_read);
	}
	
	// Initialize rest of memory
	//
	while (loc < MEMLEN) {
		cpu -> mem[loc++] = 0;
	}
	dump_memory(cpu);
}

// Get the data file to initialize memory with.  If it was
// specified on the command line as argv[1], use that file
// otherwise use default.sdc.  If file opening fails, complain
// and terminate program execution with an error.
// See linux command man 3 exit for details.
//
FILE *get_datafile(int argc, char *argv[]) {
	char *default_datafile_name = "default.sdc";
	char *datafile_name = NULL;

	// *** STUB *** set datafile name to argv[1] or default
	if (argv[1] != NULL)
	{
		datafile_name = argv[1];
	}
	else
	{
		datafile_name = default_datafile_name;
	}

	// *** STUB ***
	FILE *datafile = fopen(datafile_name, "r");
	printf("\nOpening %s...\n", datafile_name);

	// *** STUB *** if the open failed, complain and call
	if (datafile_name == NULL)
	{
		printf("Couldn't open file %s\n", datafile_name);
		exit(EXIT_FAILURE); //to quit the entire program
	}

	return datafile;
}

// dump_CPU(CPU *cpu): Print out the control unit
// and general-purpose registers
// 
void dump_CPU(CPU *cpu) {
	// *** STUB ****
	printf("-------------------------------------------------------------------------\n");
	printf("|");
	printf("  PC: %.2d    IR:% .4d    RUNNING: %d\t\t\t\t\t|", (*cpu).pc, (*cpu).ir, (*cpu).running);

	dump_registers(cpu);
}

// dump_memory(CPU *cpu): For each memory address that
// contains a non-zero value, print out a line with the
// address, the value as an integer, and the value
// interpreted as an instruction.
//
void dump_memory(CPU *cpu) {
	// *** STUB ****
	int temp_op;
	//registering each opcode to a specific instruction
	char *instr[MEMLEN];
	instr[0] = "HALT";
	instr[1] = "LD";
	instr[2] = "ST";
	instr[3] = "ADD";
	instr[4] = "NEG";
	instr[5] = "LDM";
	instr[6] = "ADDM";
	instr[7] = "BR";
	instr[8] = "BRC";
	instr[90] = "GETC";
	instr[91] = "OUT";
	instr[92] = "PUTS";
	instr[93] = "DMP";
	instr[94] = "MEM";
	instr[95] = "NOP";
	
	printf("\n\tMEMORY (non-zero values only):\n\t   @LOC\t VALUE\tINSTR");
	printf("\n\t----------------------------------");
	for (int i = 0; i < MEMLEN; i++)
	{
		if ((*cpu).mem[i] != 0)
		{
			//gets int value of only the op code
			if (abs((*cpu).mem[i]) < 9000)
			{
				(*cpu).opcode = abs((*cpu).mem[i]/1000);
				temp_op = (*cpu).opcode;
			}
			else 
			{
				(*cpu).opcode = abs((*cpu).mem[i]/100);
				temp_op = (*cpu).opcode;
			}

			//sets BRC to P or N instruction
			if ((*cpu).mem[i] > 0)
			{
				instr[8] = "BRP";
			}
			else if ((*cpu).mem[i] < 0)
			{
				instr[8] = "BRN";
			}	
			
			//gets int value of register number
			if ((temp_op == 0) || (temp_op == 7) || (temp_op >= 9))
			{
				if ((temp_op == 00) || (temp_op == 90) || (temp_op == 91) || (temp_op >= 93))
				{
					printf("\n\t|  @ %.2d\t% .4d   %s\t\t |", i, (*cpu).mem[i], instr[(*cpu).opcode]);
				}
				else
				{
					//saved MM value
					//taking into account the exceptions for negative values
					if (temp_op == 5 || temp_op == 6)
					{
						(*cpu).addr_MM = (*cpu).mem[i]%100;
					}
					else
					{
						(*cpu).addr_MM = abs((*cpu).mem[i]%100);
					}
					printf("\n\t|  @ %.2d\t% .4d   %s\t    % .2d  |", i, (*cpu).mem[i], instr[(*cpu).opcode], (*cpu).addr_MM);
				}
			}
			else
			{
				if ((temp_op == 00) || (temp_op == 90) || (temp_op == 91) || (temp_op >= 93))
				{
					printf("\n\t|  @ %.2d\t% .4d   %s\t\t |", i, (*cpu).mem[i], instr[(*cpu).opcode]);
				}
				else if (temp_op == 4)
				{
					printf("\n\t|  @ %.2d\t% .4d   %s\tR%d\t |", i, (*cpu).mem[i], instr[(*cpu).opcode], (*cpu).reg_R);
				}
				else
				{
					//saved MM value
					//taking into account the exceptions for negative values
					if (temp_op == 5 || temp_op == 6)
					{
						(*cpu).addr_MM = (*cpu).mem[i]%100;
					}
					else
					{
						(*cpu).addr_MM = abs((*cpu).mem[i]%100);
					}
					(*cpu).reg_R = abs(((*cpu).mem[i]/100)%10);
					printf("\n\t|  @ %.2d\t% .4d   %s\tR%d, % .2d  |", i, (*cpu).mem[i], instr[(*cpu).opcode], (*cpu).reg_R, (*cpu).addr_MM);
				}
			}

		}
	}
	printf("\n\t----------------------------------");
			
}


// dump_registers(CPU *cpu): Print register values in two rows of
// five.
//
void dump_registers(CPU *cpu) {
	// *** STUB ****
	printf("\n|  R0:% .2d\tR1:% .2d\t\tR2:% .2d\t\tR3:% .2d\t\tR4:% .2d  |", (*cpu).reg[0], (*cpu).reg[1], (*cpu).reg[2], (*cpu).reg[3], (*cpu).reg[4]);
	printf("\n|  R5:% .2d\tR6:% .2d\t\tR7:% .2d\t\tR8:% .2d\t\tR9:% .2d  |", (*cpu).reg[5], (*cpu).reg[6], (*cpu).reg[7], (*cpu).reg[8], (*cpu).reg[9]);
	printf("\n-------------------------------------------------------------------------");
}
