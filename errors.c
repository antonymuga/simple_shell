#include "shell.h"

int status;

int lineNumber;

char *nameOfShell;

/**
 * displayErrorMessage - display error messages
 * @errorCausingCommand: the command that caused the error
 * @errorCausingCommandArguement: the arguement 1 for the command
 */
void displayErrorMessage(char *errorCausingCommand, char *errorCausingCommandArguement)
{
	char *errorStringNumber = _itoa(lineNumber);

	write(STDERR_FILENO, nameOfShell, getStringLength(nameOfShell));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, errorStringNumber, getStringLength(errorStringNumber));
	free(errorStringNumber);

	if (compareString("cd", errorCausingCommand, EQUAL) == POSITIVE)
	{
		status = 2;
		write(STDERR_FILENO, ": cd: Error, can't cd to", 17);
		write(STDERR_FILENO, errorCausingCommandArguement, getStringLength(errorCausingCommandArguement));
		write(STDERR_FILENO, "\n", 1);
		return;
	}

	if (compareString("exit", errorCausingCommand, EQUAL) == POSITIVE)
	{
		write(STDERR_FILENO, ": exit: Not Allowed: ", 24);
		write(STDERR_FILENO, errorCausingCommandArguement, getStringLength(errorCausingCommandArguement));
		write(STDERR_FILENO, "\n", 1);
		return;
	}
	if (*errorCausingCommand == ';' || *errorCausingCommand == '|' || *errorCausingCommand == '&')
	{
		status = 2;
		write(STDERR_FILENO, ": Syntax error: \"", 17);
		write(STDERR_FILENO, errorCausingCommand, 1);
		if (*errorCausingCommand == *(errorCausingCommand + 1))
			write(STDERR_FILENO, errorCausingCommand, 1);
		write(STDERR_FILENO, "\" unexpected\n", 14);
		return;
	}

	status = 127;
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, errorCausingCommand, getStringLength(errorCausingCommand));
	write(STDERR_FILENO, ": not found\n", 12);
}


/**
 * errorCheckerForInput - checks for unwanted characters
 * @pointer: pointer to segment that should be checked
 * Return: POSITIVE for no error, NEGATIVE for error
 */

int errorCheckerForInput(char *pointer)
{
	char *looper = pointer;

	looper++;
	if (*pointer == ';' && *looper == *pointer)
	{
		displayErrorMessage(pointer, NULL);
		return (NEGATIVE);
	}
	if (*looper == *pointer)
		looper++;

	while (*looper == ' ')
		looper++;

	if (*looper == ';' || *looper == '|' || *looper == '&')
	{
		displayErrorMessage(looper, NULL);
		return (NEGATIVE);
	}

	return (POSITIVE);
}

/**
 * inputCleaner - cleans up command line input
 * @previousBuffer: buffer to be cleaned up
 * @previousSize: the size of the prvious buffer
 *
 * Return: the new, sanitized buffer
 */
char *inputCleaner(char *previousBuffer, size_t *previousSize)
{
	char *currentBuffer = malloc(*previousSize * 3);
	char *newPointer = currentBuffer;
	char *oldPointer = previousBuffer;

	while (*oldPointer != '\0')
	{
		while (*oldPointer == ' ')
			oldPointer++;
		while (*oldPointer	!= ' ' && *oldPointer != ';' && *oldPointer != '|'
		       && *oldPointer != '&' && *oldPointer != '\0')
		{
			*newPointer = *oldPointer;
			newPointer++;
			oldPointer++;
		}
		while (*oldPointer == ' ')
			oldPointer++;
		if (newPointer != currentBuffer && *oldPointer != '\0')
		{
			*newPointer = ' ';
			newPointer++;
		}

		if (*oldPointer == ';' || *oldPointer == '|' || *oldPointer == '&')
		{
			if (errorCheckerForInput(oldPointer) == NEGATIVE)
			{
				*previousSize = 0;
				break;
			}
			*newPointer = *oldPointer;
			if (*oldPointer == ';')
			{
				newPointer++;
				*newPointer = ' ';
			}
			else if (*(oldPointer + 1) == *oldPointer)
			{
				if (newPointer == currentBuffer)
				{
					displayErrorMessage(oldPointer, NULL);
					*previousSize = 0;
					break;
				}
				newPointer++;
				*newPointer = *oldPointer;
				newPointer++;
				*newPointer = ' ';
				oldPointer++;
			}
			newPointer++;
			oldPointer++;
		}
	}
	*newPointer = '\0';
	free(previousBuffer);
	return (currentBuffer);
}