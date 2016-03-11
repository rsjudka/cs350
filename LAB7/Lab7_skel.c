// ** Skeleton ** <-- remove this line and complete the program
// *** Your name and section (replace this with the correct information ***
// (Also rename this file to Lab04_YourName_Section.c and remove this comment line)

// CS 350, Spring 2016
// Lab 7: SDC Simulator, part 2
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
    // *** STUB *** Lab 6 items omitted ....

    int read_execute_command(CPU *cpu);
    int execute_command(char cmd_char, CPU *cpu);
    void help_message(void);
    void many_instruction_cycles(int nbr_cycles, CPU *cpu);
    void one_instruction_cycle(CPU *cpu);

    void exec_HLT(CPU *cpu);
    // *** STUB ***

// Main program: Initialize the cpu, and read the initial memory values
//
int main(int argc, char *argv[]) {
    printf("SDC Simulator pt 2 skeleton: CS 350 Lab 7\n");
    CPU cpu_value, *cpu = &cpu_value;

// *** STUB *** Lab 6 items omitted (initialize CPU and memory) ....
    printf("*** STUB *** Lab 6 items omitted (initialize CPU and memory)\n");

    char *prompt = "> ";
    printf("\nBeginning execution; type h for help\n%s", prompt);

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

    read_success = fgets(cmd_line, CMD_LINE_LEN, stdin);

    // *** STUB ***  Quit if read_success indicates end-of-file

    // *** STUB ***
    // while not done,
    //      Handle q, h, ?, d commands, integer (nbr of instruction cycles),
    //          or empty line (one instruction cycle)
    //      Read next command line

    return done;
}

// Execute a nonnumeric command; complain if it's not 'h', '?', 'd', 'q' or '\n'
// Return true for the q command, false otherwise
//
int execute_command(char cmd_char, CPU *cpu) {
    if (cmd_char == '?' || cmd_char == 'h') {
        help_message();
    }
    // *** STUB ****
    return 0;
}

// Print standard message for simulator help command ('h' or '?')
//
void help_message(void) {
    // *** STUB ****
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
}

// Execute one instruction cycle
//
void one_instruction_cycle(CPU *cpu) {
    // If the CPU isn't running, say so and return.
    // If the pc is out of range, complain and stop running the CPU.
    //
    // *** STUB ****

    // Get instruction and increment pc
    //
    int instr_loc = cpu -> pc;  // Instruction's location (pc before increment)
    cpu -> ir = cpu -> mem[cpu -> pc];
    (cpu -> pc)++;

    // Decode instruction into opcode, reg_R, addr_MM, and instruction sign
    //
    // *** STUB ****

    // Echo instruction
    // *** STUB ***

    switch (cpu -> opcode) {
    case 0: exec_HLT(cpu); break;
    // *** STUB ****
    default: printf("Bad opcode!? %d\n", cpu -> opcode);
    }
}

// Execute the halt instruction (make CPU stop running)
//
void exec_HLT(CPU *cpu) {
    printf("HALT\nHalting\n");
    cpu -> running = 0;
}