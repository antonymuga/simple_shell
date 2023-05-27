#include "shell.h"

int status;

char *nameOfShell;

/**
 * logicalOperatorHandler - handles logical operators
 * @arguements: commands
 * @operator: the initial character of a logical operator
 * @last_compare: boolean of whether last command evaluated to POSITIVE or NEGATIVE
 * Return: if this command evaluates to POSITIVE or NEGATIVE
 */

int logicalOperatorHandler(char **arguements, char operator, int last_compare)
{
	int i;

	if (last_compare == NONE)
	{
		i = runCommand(arguements);
		if (i == TERMINATE_PROCESS_EXECUTION)
			return (TERMINATE_PROCESS_EXECUTION);
		if (i == POSITIVE)
			return (POSITIVE);

		return (NEGATIVE);
	}
	if (last_compare == POSITIVE && operator == '&')
	{
		i = runCommand(arguements);
		if (i == TERMINATE_PROCESS_EXECUTION)
			return (TERMINATE_PROCESS_EXECUTION);
		if (i == POSITIVE)
			return (POSITIVE);

		return (NEGATIVE);
	}

	if (last_compare == NEGATIVE && operator == '|')
	{
		i = runCommand(arguements);
		if (i == TERMINATE_PROCESS_EXECUTION)
			return (TERMINATE_PROCESS_EXECUTION);
		if (i == POSITIVE)
			return (POSITIVE);

		return (NEGATIVE);
	}

	if (last_compare == POSITIVE && operator == '|')
		return (POSITIVE);

	return (NEGATIVE);
}
