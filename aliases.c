#include "shell.h"

int status;

/**
 * commandAliasProcessing - handles command aliases
 * @arguements: command line arguements
 * @toBeFreed: boolean for whether aliases need to be freed on shell exit;
 *
 * Return: POSITIVE for exit, NEGATIVE if not alias,
 * DONT_CREATE_NEW_CHILD_PROCESS for success
 */

int commandAliasProcessing(char **arguements, int toBeFreed)
{
	static alias headNode = {NULL, NULL, NULL};
	int nullError = POSITIVE;

	if (toBeFreed)
		return (releaseAlias(headNode.nextNode));

	if (!compareString("alias", *arguements, EQUAL))
		return (replaceIfAlias(arguements, headNode.nextNode));

	if (!arguements[1])
		return (displayAliases(headNode.nextNode));

	while (*arguements)
	{
		char *characterPointer = *arguements;
		while (*characterPointer && *characterPointer != '=')
			characterPointer++;

		if (*characterPointer == '\0' || characterPointer == *arguements)
			nullError &= displayAliasValue(*arguements++, &headNode);
		else
		{
			*characterPointer++ = '\0';
			configureAlias(*arguements++, &headNode, characterPointer);
			*(characterPointer - 1) = '=';
		}
	}

	status = 0;
	return (DONT_CREATE_NEW_CHILD_PROCESS);
}

/**
 * replaceIfAlias - replaces command with it value if alias
 * @arguements: argument to be tested
 * @aliasPointer: pointer to list of aliases
 *
 * Return: POSITIVE
 */
int replaceIfAlias(char **arguements, alias *aliasPointer)
{
	while (aliasPointer != NULL)
	{
		if (compareString(*arguements, aliasPointer->nodeValue, EQUAL) == POSITIVE)
		{
			*arguements = copyString(aliasPointer->nodeValue);
			return (LAUNCH_RUNTIME_PROCESS);
		}
		aliasPointer = aliasPointer->nextNode;
	}
	return (POSITIVE);
}

/**
 * releaseAlias - frees up resources used by aliases
 * @aliasPointer: points to headNode of the alias list
 *
 * Return: POSITIVE
 */
int releaseAlias(alias *aliasPointer)
{
	alias *temporary;

	while (aliasPointer != NULL)
	{
		temporary = aliasPointer;
		aliasPointer = aliasPointer->nextNode;
		free(temporary->nodeValue);
		free(temporary->nodeValue);
		free(temporary);
	}

	return (POSITIVE);
}

/**
 * displayAliases - print all alises in list
 * @aliasPointer: pointer to list of aliases
 *
 * Return: DONT_CREATE_NEW_CHILD_PROCESS
 */
int displayAliases(alias *aliasPointer)
{
	while (aliasPointer != NULL)
	{
		write(STDOUT_FILENO, aliasPointer->nodeValue, getStringLength(aliasPointer->nodeValue));
		write(STDOUT_FILENO, "=\'", 2);
		write(STDOUT_FILENO, aliasPointer->nodeValue,
		      getStringLength(aliasPointer->nodeValue));
		write(STDOUT_FILENO, "\'\n", 2);
		aliasPointer = aliasPointer->nextNode;
	}
	return (DONT_CREATE_NEW_CHILD_PROCESS);
}


/**
 * displayAliasValue - print alias value
 * @arguement: alias name
 * @aliasPointer: pointer to a list of aliases
 *
 * Return: POSITIVE if alias, NEGATIVE if not alias
 */
int displayAliasValue(char *arguement, alias *aliasPointer)
{
	while (aliasPointer != NULL)
	{
		fflush(stdin);
		if (compareString(arguement, aliasPointer->nodeValue, EQUAL) == POSITIVE)
		{
			write(STDOUT_FILENO, arguement, getStringLength(arguement));
			write(STDOUT_FILENO, "=\'", 2);
			write(STDOUT_FILENO, aliasPointer->nodeValue,
			      getStringLength(aliasPointer->nodeValue));
			write(STDOUT_FILENO, "\'\n", 2);
			return (POSITIVE);
		}
		aliasPointer = aliasPointer->nextNode;
	}

	status = 1;
	write(STDERR_FILENO, "alias: ", 7);
	write(STDERR_FILENO, arguement, getStringLength(arguement));
	write(STDERR_FILENO, " not found\n", 11);

	return (NEGATIVE);
}

/**
 * configureAlias - sets value of an alias
 * @arguement: alias name
 * @aliasPointer: pointer alias list
 * @setValue: value to set for alias
 *
 * Return: POSITIVE
 */
int configureAlias(char *arguement, alias *aliasPointer, char *setValue)
{
	while (aliasPointer->nextNode != NULL
	       && compareString(aliasPointer->nodeValue, arguement, EQUAL) != POSITIVE)
	{
		aliasPointer = aliasPointer->nextNode;
	}

	if (compareString(aliasPointer->nodeValue, arguement, EQUAL) == POSITIVE)
	{
		free(aliasPointer->nodeValue);
	}
	else
	{
		aliasPointer->nextNode = malloc(sizeof(alias *));
		aliasPointer = aliasPointer->nextNode;
		if (aliasPointer == NULL)
			exit(EXIT_FAILURE);

		aliasPointer->nodeValue = copyString(arguement);
		aliasPointer->nextNode = NULL;
	}
	aliasPointer->nodeValue = copyString(setValue);

	return (POSITIVE);
}
