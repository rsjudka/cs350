//Robert Judka
//CS 350, Section 01
//Lab 03 - Other Bases and Data

// CS 350, Spring 2016
// Lab 3 -- Converting a natural number from decimal to a given base
//
// Illinois Institute of Technology, (c) 2016, James Sasaki

// This program repeatedly reads in an integer and base
// (both in decimal), converts the integer from decimal
// to the requested base, and prints the result.  We stop
// if the read-in integer value is < 1 or the base is < 2.

#include <stdio.h>

// Define array length to be large enough to hold the
// longest possible output (max positive int in base 2)
#define ARRAYLEN 32

int main() 
{
	printf("Robert Judka\nCS 350-01\nLab 03 - Other Bases and Data\n\n\n");
	printf("Welcome to the Super Cool Extravagent Base Converter Tool*!\n");
	printf("\t\t*limited to base 36");

	// Prompt user for value and base
	int value;
	int base;
	printf("\n\nEnter an integer (must be >= 1): ");
	scanf("%d", &value);
	printf("Enter a base term (must be >= 2 & <= 36): ");
	scanf("%d", &base);
	
	//save original value
	int ogValue = value;
	int ogBase = base;
	
	//Loops until user inputs value < 1 and base < 2 or > 36
	while (value >= 1 && (base >= 2 && base <= 36))
	{
		//max int of value
		if (ogValue > 2147483647) 
		{
			printf("Oh Snap :( You expect too much of me");
			printf(" \t %d is too large of a number to convert", ogValue);
		}
		else
			{
			//variables to convert to new base
			int toConvert = value;
			int counter = 0;
			int remainder = 0;
			int digits[ARRAYLEN];
			int i = 0;
		
			//Loops until toConvert no longer divisible by base
			//Finds size of bitstring
			do
			{
				toConvert = toConvert / base;
				counter++;
			}
			while (toConvert != 0);
		
			//saves each value of the converted number into digits[]
			do
			{
				remainder = value % base;
				value = value / base;
				digits[i] = remainder;
				i++;
			}
			while (i < counter);
			
			//array of all possible bits
			char prt_digits[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			
			//loops in reverse and prints out bitstring
			printf("\n\t%d in base %d is: ",ogValue, ogBase);
			for (int i = counter - 1; i >= 0; i--)
			{
				printf("%c", prt_digits[digits[i]]);
			}

			//prompt user again
			printf("\n\n\nEnter an integer (must be >= 1): ");
			scanf("%d", &value);
			printf("Enter a base term (must be >= 2 & <= 36): ");
			scanf("%d", &base);
			
			//save new data
			ogValue = value;
			ogBase = base;
			}
	}
	
	printf("\nYou have now left the Converter Tool. Have a great day!\n");
}