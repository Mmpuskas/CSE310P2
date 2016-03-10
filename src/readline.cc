#include <stdio.h>

/*
 * Reads a line from stdin
 * Parameters:
 * 	char *linePointer - Pointer to the beginning of the line to be read
 */
void readLine(char *linePointer)
{
	if (linePointer != NULL)
	{
		int c = EOF;
		unsigned int i = 0;

		while ((c = getchar()) != '\n' && c != EOF)
		{
			linePointer[i++] = (char) c;

		}
		linePointer[i] = '\0';
	}
}
