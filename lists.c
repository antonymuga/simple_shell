#include "shell.h"

int status;

/**
 * lengthOfList - gets length of a testList
 * @testList: the testList to find length of
 * @indexedEntry: the testList indexedEntry to be indexed
 * Return: length or index if success, -1 if failure
 */
int lengthOfList(char **testList, char *indexedEntry)
{
	int i = 0;

	if (indexedEntry == NULL)
	{
		while (*testList != NULL)
		{
			i++;
			testList++;
		}
		i++;
		return (i);
	}
	else
	{
		while (*testList != NULL)
		{
			if (compareString(*testList, indexedEntry, PREFIX) == POSITIVE)
				return (i);

			i++;
			testList++;
		}
	}

	return (-1);
}