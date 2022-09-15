#include"usefulFunctions.h"
#include<stdio.h>

/* Functions Implementations */

/* A function to read input string with fgets and deal with "new line" issue */
void readString(uint8_t* strRead, uint8_t strSize) {
	uint8_t bufferInput[30]; /* 30 is suitable in this application */
	fgets(bufferInput, 30, stdin);

	uint8_t index;
	/* Stop at either the "new line" character or at the end of the required array */
	for (index = 0; index <= (strSize - 1); index++)
	{
		/* If we find a new line, exit the loop and do not store it */
		if ((bufferInput[index] == '\n'))
		{
			break;
		}
		strRead[index] = bufferInput[index];
	}
	/* if we still within the array size, terminate with null */
	if (index <= (strSize - 1))
	{
		strRead[index] = '\0';
	}
}