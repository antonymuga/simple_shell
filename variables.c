#include "shell.h"

int status;

int lineNumber;

char *nameOfShell;

/**
 * checkIfArgumentIsVariable - tests if an argument is a variable and replaces with its value if true
 * @arguement: argument to test
 * Return: pointer to tested arguement
 */

char *checkIfArgumentIsVariable(char *arguement)
{
	char *temporary;
	char *duplicate = NULL;
	char *pointer = arguement;
	int i;
	char *nextNode;
	int isAVariable;
	

	while (*pointer != '\0')
	{
		if (*pointer == '$')
		{
			if (duplicate == NULL)
			{
				duplicate = copyString(arguement);
				i = pointer - arguement;
				pointer = duplicate + i;
			}
			nextNode = pointer + 1;
			while (*nextNode != '\0' && *nextNode != '$' && *nextNode != '#')
				nextNode++;
			if (*nextNode == '$' && nextNode > pointer + 1)
				isAVariable = POSITIVE;
			else if (*nextNode == '#')
				isAVariable = NONE;
			else
				isAVariable = NEGATIVE;

			*nextNode = '\0';

			if (compareString("$?", pointer, EQUAL) == POSITIVE)
				temporary = _itoa(status);
			else if (compareString("$0", pointer, EQUAL) == POSITIVE)
				temporary = copyString(nameOfShell);
			else if (findArrayElement(environ, pointer + 1) != NULL)
				temporary = copyString(findArrayElement
					      (environ, pointer + 1)
					      + getStringLength(pointer));
			else
				temporary = copyString("");

			*pointer = '\0';
			pointer = concatenateString(duplicate, temporary);
			free(temporary);
			if (isAVariable == NEGATIVE)
			{
				free(duplicate);
				duplicate = pointer;
				break;
			}
			if (isAVariable == POSITIVE)
				*nextNode = '$';
			else if (isAVariable == NONE)
				*nextNode = '#';
			temporary = concatenateString(pointer, nextNode);
			free(pointer);
			pointer = temporary;
			free(duplicate);
			duplicate = pointer;
			if (isAVariable == NONE)
			{
				while (*pointer != '#')
					pointer++;
			}
		}
		pointer++;
	}
	if (duplicate != NULL)
		return (duplicate);

	return (arguement);
}
