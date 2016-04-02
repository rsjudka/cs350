//Robert Judka
// CS 350, Spring 2016
// Final Project, part 1

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
	} CPU;
		
// Prototypes
	int main(int argc, char *argv[]);
	void initialize_CPU(CPU *cpu);
	void initialize_memory(int argc, char *argv[], CPU *cpu);
	FILE *get_datafile(int argc, char *argv[]);

	void dump_CPU(CPU *cpu);
	void dump_memory(CPU *cpu);
	void dump_registers(CPU *cpu);

//Main program: Initialize the cpu, and read the initial memory values
int main(int argc, char *argv[]) {
	printf("Robert Judka CS350-01\n");
	printf("LC-3 Simulator: Final Project pt. 1\n");
	CPU cpu_value, *cpu = &cpu_value;
	initialize_CPU(cpu);
	initialize_memory(argc, argv, cpu);
	printf("\nINITIAL CPU:\n");	
	dump_CPU(cpu);
	printf("\n");
	dump_memory(cpu);
	return 0;
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
			(*cpu).pc = (*cpu).orig;
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
	printf("PC: x%.4hx\tIR: x%.4d\tCC: %c\t     RUNNING: %d", (*cpu).pc, (*cpu).ir, (*cpu).cc, (*cpu).running);

	dump_registers(cpu);
}

// prints memory info for any memory locations not containing a zero
void dump_memory(CPU *cpu) {
	Word temp_loc = (*cpu).orig;
	
	printf("\nMEMORY (non-zero values from x0000 - xFFFF):\n");
	for (int i = 0; i < MEMLEN; i++)
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
