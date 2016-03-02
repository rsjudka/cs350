//Robert Judka
// CS 350, Spring 2016
// Lab 6: SDC Simulator, part 1

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
	int main(int argc, char *argv[]);
	void initialize_CPU(CPU *cpu);
	void initialize_memory(int argc, char *argv[], CPU *cpu);
	FILE *get_datafile(int argc, char *argv[]);

	void dump_CPU(CPU *cpu);
	void dump_memory(CPU *cpu);
	void dump_registers(CPU *cpu);

// Main program: Initialize the cpu, and read the initial memory values
//
int main(int argc, char *argv[]) {
	printf("Robert Judka CS350-01\n");
	printf("SDC Simulator: Lab 6\n");
	CPU cpu_value, *cpu = &cpu_value;
	initialize_CPU(cpu);
	initialize_memory(argc, argv, cpu);
	printf("\n");

	// That's it for Lab 6
	//
	return 0;
}


// Initialize the CPU (pc, ir, instruction sign, running flag,
// and the general-purpose registers).
//
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
			}
			else
			{
				cpu -> mem[loc] = value_read;
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
	printf("------------------------------------------------\n");
	printf("|");
	printf("  PC: %.2d    IR: %.4d    RUNNING: %d\t       |", (*cpu).pc, (*cpu).ir, (*cpu).running);

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
	printf("\n|  R0: %d\tR1: %d\tR2: %d\tR3: %d\tR4: %d  |", (*cpu).reg[0], (*cpu).reg[1], (*cpu).reg[2], (*cpu).reg[3], (*cpu).reg[4]);
	printf("\n|  R5: %d\tR6: %d\tR7: %d\tR8: %d\tR9: %d  |", (*cpu).reg[5], (*cpu).reg[6], (*cpu).reg[7], (*cpu).reg[8], (*cpu).reg[9]);
	printf("\n------------------------------------------------");
}

//I MADE AWESOME BOXES!!!!!!!!
//I LIKE BOXES
//like ocd with boxes...