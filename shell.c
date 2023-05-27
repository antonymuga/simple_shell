#include "shell.h"

int status = 0;

int lineNumber = 1;

char *nameOfShell = NULL;

/**
 * main - runs the core logic of the shell
 * @argc: argc, number of cmd arguments
 * @argv: argv, array of cmd arguements
 * Return: status code 0
 */

int main(__attribute__((unused))int argc, char **argv)
{
	int numberOfBytesRead;
	int separation = NEGATIVE;
	int i;
	size_t sizeOfBuffer = 1;
	char *buffer = NULL;
	char *bufferPointer;
	char *temporaryBuffer;
	char **arguements = NULL;

	nameOfShell = copyString(*argv);

	environ = duplicateArray(environ, lengthOfList(environ, NULL));

	signal(SIGINT, SIG_IGN);

	buffer = malloc(1);
	if (buffer == NULL)
		exit(EXIT_FAILURE);

	while (1)
	{
		if (separation == NEGATIVE)
		{
			if (isatty(STDIN_FILENO) == 1)
				write(STDOUT_FILENO, "alx_shell$ ", 11);

			numberOfBytesRead = getline(&buffer, &sizeOfBuffer, stdin);

			if (numberOfBytesRead == -1)
				break;
			if (numberOfBytesRead == 1)
			{
				lineNumber++;
				continue;
			}
			buffer[numberOfBytesRead - 1] = '\0';
			buffer = inputCleaner(buffer, &sizeOfBuffer);
			if (sizeOfBuffer == 0)
			{
				lineNumber++;
				continue;
			}
			bufferPointer = buffer;
		}
		else
			bufferPointer = temporaryBuffer;

		temporaryBuffer = NULL;
		arguements = createArray(bufferPointer, ' ', &temporaryBuffer);
		if (temporaryBuffer != NULL)
			separation = POSITIVE;
		else
			separation = NEGATIVE;

		i = commandRuntimeHandler(arguements);

		free(arguements);

		if (separation == NEGATIVE)
			lineNumber++;

		if (i == TERMINATE_PROCESS_EXECUTION)
			break;
	}
	free(buffer);
	commandAliasProcessing(NULL, POSITIVE);
	releaseArray(environ);
	free(nameOfShell);

	return (status % 256);
}
