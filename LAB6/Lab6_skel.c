// ** Skeleton ** <-- remove this line and complete the program
// *** Your name and section (replace this with the correct information ***
// (Also rename this file to Lab04_YourName_Section.c and remove this comment line)

// CS 350, Spring 2016
// Lab 6: SDC Simulator, part 1
//
// Illinois Institute of Technology, (c) 2016, James Sasaki

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
	printf("SDC Simulator pt 1 skeleton: CS 350 Lab 6\n");
	CPU cpu_value, *cpu = &cpu_value;
	initialize_CPU(cpu);
	initialize_memory(argc, argv, cpu);

	// That's it for Lab 6
	//
	return 0;
}


// Initialize the CPU (pc, ir, instruction sign, running flag,
// and the general-purpose registers).
//
void initialize_CPU(CPU *cpu) {
	
	// *** STUB ***
	
	printf("\nInitial CPU:\n");
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
		words_read = sscanf(buffer, "%d", &value_read);

		// *** STUB *** set memory value at current location to
		// value_read and increment location.  Exceptions: If
		// loc is out of range, complain and quit the loop. If
		// value_read is outside -9999...9999, then it's a
		// sentinel and we should say so and quit the loop.
		
		// Get next line and continue the loop
		//
		// *** STUB ***
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
	datafile_name = default_datafile_name;	// *** STUB ***

	FILE *datafile = fopen(datafile_name, "r");

	// *** STUB *** if the open failed, complain and call
	// exit(EXIT_FAILURE); to quit the entire program

	return datafile;
}

// dump_CPU(CPU *cpu): Print out the control unit
// and general-purpose registers
// 
void dump_CPU(CPU *cpu) {
	// *** STUB ****
	dump_registers(cpu);
}

// dump_memory(CPU *cpu): For each memory address that
// contains a non-zero value, print out a line with the
// address, the value as an integer, and the value
// interpreted as an instruction.
//
void dump_memory(CPU *cpu) {
	// *** STUB ****
}


// dump_registers(CPU *cpu): Print register values in two rows of
// five.
//
void dump_registers(CPU *cpu) {
	// *** STUB ****
}