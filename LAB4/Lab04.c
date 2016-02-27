//Robert Judka
//CS 350, Section 01
//Lab 04 - Breaking Up a Bitstring

// This program processes each line of a file; the filename is
// specified as a command-line argument (argv[1]) or defaults to
// Lab4_data.txt. Each line should contain a hex integer value and
// two decimal integers, len1 and len2. We find the leftmost len1
// bits of the value, the next len2 bits of the value, and the
// rightmost len3 = (32 - (len1 + len2)) remaining bits. (All 3
// values should be > 0 else we signal an error and skip to the
// next line of input.)  We print the three bitstrings in hex and
// in decimal. The leftmost string is unsigned, the middle string
// is in 1's complement, and the rightmost string is in 2's
// complement.

# include <stdio.h>
# include <math.h>

int main(int argc, char *argv[])
{
	//prints out the file executed and the name of the .txt file
	printf("The commandeth line hath %d part(s):\n", argc);
	
	for (int i = 0; i < argc; i++)
		printf("argv[%d] = %s\n", i, argv[i]);
		
	if (argc != 2)
	{
		printf("\nBad commandeth; shouldst be %s filename\n", argv[0]);
		return 1;
	}
	
	//opens file
	//sets fileName, if no name found, sets to "Lab4_data.txt
	char *fileName;
	if (argv[1] != NULL)
	{
		fileName = argv[1];
	}
	else
	{
		fileName = "Lab4_data.txt";
	}
	
	FILE *fileData;
	fileData = fopen(fileName, "r");
	
	//if fnf, prints error
	if (fileData == NULL)
	{
		printf("Couldn't openeth fileth %s\n", fileName);
		return 1;
	}
	
	//scan data in from file
	int value, length1, length2, length3;
	int numOfValues;
	
	printf("\n");
	
	const int base = 2; //base of binary
	unsigned x1, x2, x3;
	unsigned mask = 0xffffffff;
	int decx2, decx3;
		
	
	numOfValues = fscanf(fileData, "%x %d %d", &value, &length1, &length2);
	while (numOfValues == 3)
	{
		printf("****************************************************************************************\n\n");
		length3 = 32 - (length1 + length2);
		
		if (!(length1 > 0) || !(length2 > 0) || !(length3 > 0))
		{
			char length1Error = ' ';
			char length2Error = ' ';
			char length3Error = ' ';
			if (length1 <= 0)
				length1Error = '*';
			else if (length2 <= 0)
				length2Error = '*';
			else
				length3Error = '*';
				
			printf("Hex value: %x\n", value);
			printf("Err'r: not all lengths positive\n");
			printf("\t%cLength 1: %d\n\t%cLength 2: %d\n\t%cLength 3: %d\n", length1Error, length1, length2Error, length2, length3Error, length3);
		}
		else
		{
			mask = 0xffffffff;
			mask = mask >> (length1 + length2);
			x3 = value & mask; 

			mask = 0xffffffff;
			mask = mask >> (length1 + length3);
			mask = mask << length3;
			x2 = value & mask;
			x2 = x2 >> length3;
		
			mask = 0xffffffff;
			mask = mask >> (length2 + length3);
			mask = mask << (length2 + length3);
			x1 = value & mask;
			x1 = x1 >> (length2 + length3);
		
			decx2 = x2 - pow(base, length2);
			decx2 = decx2 + 1;
			decx3 = x3 - pow(base, length3);
		
			printf("Hex value: %#08x\nInt value: %u\n", value, value);
			printf("\nIts leftmost\t%2d bits art: %#x\t====>\t%d\tin unsign'd magnitude\n", length1, x1, x1);
			printf("Its next\t%2d bits art: %#x\t====>\t%d\tin 1's complement\n", length2, x2, decx2);
			printf("Its remaining\t%2d bits art: %#x\t====>\t%d\tin 2's complement\n", length3, x3, decx3);
		}
		
		printf("\n");
		numOfValues = fscanf(fileData, "%x %d %d", &value, &length1, &length2);
	}
	
	int endScan = fclose(fileData);
	if (endScan)
	{
		printf("Fileth closeth fail'd!\n");
	}
	return 0;
	//got bored, wrote my output in old english
	//hopefully you enjoyed it!! I'm sure you get really bored grading all these labs lol
	//also tried making it super easy to read!!!
}