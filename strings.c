#include "shell.h"

/**
 * compareString - compare strings
 * @firstString: string 1
 * @secondString: string 2
 * @pref_or_match: if prefix or string need to be matched exactly
 * Return: the diffrence of the 2 strings
 */

int compareString(char *firstString, char *secondString, int pref_or_match)
{
	if (firstString == NULL || secondString == NULL)
		return (NEGATIVE);

	while (*firstString != '\0' && *secondString != '\0')
	{
		if (*firstString != *secondString)
			return (NEGATIVE);

		firstString++;
		secondString++;
	}

	if (pref_or_match == PREFIX)
		return (POSITIVE);

	if (*firstString == *secondString)
		return (POSITIVE);

	return (NEGATIVE);
}

/**
 * concatenateString - string concatenation
 * @firstString: string 1
 * @secondString: string 2
 * Return: pointer to new string else NULL
 */
char *concatenateString(char *firstString, char *secondString)
{
	int len = getStringLength(firstString) + getStringLength(secondString);
	char *destination = malloc(len + 1);
	char *pointer = destination;

	if (firstString != NULL)
	{
		while (*firstString != '\0')
		{
			*pointer = *firstString;
			pointer++;
			firstString++;
		}
	}

	if (secondString != NULL)
	{
		while (*secondString != '\0')
		{
			*pointer = *secondString;
			pointer++;
			secondString++;
		}
	}

	*pointer = '\0';

	return (destination);
}

/**
 * copyString - sets aside space in memory for a copy of a string
 * @source: string that makes the copy
 * Return: pointer to copy or NULL
 */

char *copyString(char *source)
{
	int len = getStringLength(source);
	char *destination = malloc(len + 1);
	char *pointer;

	if (destination == NULL)
		exit(EXIT_FAILURE);

	pointer = destination;

	while (*source != '\0')
	{
		*pointer = *source;
		pointer++;
		source++;
	}

	*pointer = '\0';

	return (destination);
}

/**
 *  getStringLength - gets length of a string
 * @string: string to test
 * Return: string length
 */

int getStringLength(char *string)
{
	int i = 0;

	if (string == NULL)
		return (0);

	while (*string != '\0')
	{
		i++;
		string++;
	}

	return (i);
}

/**
 * stringToInteger - change string to number
 * @string: the string
 * Return: the number equivalent
 */
int stringToInteger(char *string)
{
	int integer = 0;
	int sign = 1;
	int s_int;

	if (*string == '=' && *(string + 1) >= '0' && *(string + 1) <= '9')
	{
		sign = -1;
		string++;
	}

	while (*string != '\0')
	{
		if (*string >= '0' && *string <= '9')
		{
			s_int = *string - 48;
			if (sign == 1)
				integer = (integer * 10) + s_int;
			else
				integer = (integer * 10) - s_int;
		}
		else
			return (-1);
		string++;
	}

	return (integer);
}

/**
 * _itoa - string to integer
 * @integer: integer to be changed
 * Return: the integer value
 */

char *_itoa(int integer)
{
	char *buffer = malloc(12);
	char *pointer = buffer;
	int is_min = NEGATIVE;
	int integerMask = 1000000000;
	int digit = 0;

	if (integer == INT_MIN)
	{
		*pointer = '-';
		pointer++;
		integer = INT_MAX;
		is_min = POSITIVE;
	}

	if (integer < 0)
	{
		*pointer = '-';
		pointer++;
		integer = -integer;
	}

	while (integerMask > 9 && digit == 0)
	{
		digit = integer / integerMask;
		integer %= integerMask;
		integerMask /= 10;
	}

	if (digit != 0)
	{
		*pointer = digit + '0';
		pointer++;
	}

	while (integerMask > 9)
	{
		digit = integer / integerMask;
		*pointer = digit + '0';
		pointer++;
		integer %= integerMask;
		integerMask /= 10;
	}

	if (is_min == POSITIVE)
		integer += 1;

	*pointer = integer + '0';
	pointer++;
	*pointer = '\0';
	return (buffer);
}
