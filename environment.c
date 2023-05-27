#include "shell.h"

int status;

/**
 * displayEnvironMentVariable - prints the environment
 * Return: POSITIVE
 */

int displayEnvironMentVariable(void)
{
	char **pointer = environ;

	while (*pointer != NULL)
	{
		write(STDOUT_FILENO, *pointer, getStringLength(*pointer));
		write(STDOUT_FILENO, "\n", 1);
		pointer++;
	}

	status = 0;

	return (DONT_CREATE_NEW_CHILD_PROCESS);
}

/**
 * configureEnvironmentVariable - sets environment dependencies
 * @name: vabriable name
 * @value: variable value
 * Return: status code 0
 */

int configureEnvironmentVariable(const char *name, const char *value)
{
	char **createdEnvironment;
	char *buffer;
	char *temporaryBuffer;
	char *element_pointer = findArrayElement(environ, (char *) name);
	int len;

	if (value == NULL)
	{
		write(STDERR_FILENO, "setenv: no value given\n", 23);
		status = 2;
		return (DONT_CREATE_NEW_CHILD_PROCESS);
	}

	buffer = concatenateString((char *)name, "=");
	temporaryBuffer = concatenateString(buffer, (char *)value);
	free(buffer);
	buffer = temporaryBuffer;

	if (element_pointer == NULL)
	{
		len = lengthOfList(environ, NULL);
		createdEnvironment = duplicateArray(environ, len + 1);
		createdEnvironment[len - 1] = buffer;
		createdEnvironment[len] = NULL;
		releaseArray(environ);
		environ = createdEnvironment;
		return (DONT_CREATE_NEW_CHILD_PROCESS);
	}

	len = lengthOfList(environ, (char *)name);
	free(environ[len]);
	environ[len] = buffer;

	status = 0;

	return (DONT_CREATE_NEW_CHILD_PROCESS);
}


/**
 * deleteEnvironmentVariable - removes an environment variable
 * @name: variable name
 * Return: status code 0
 */
int deleteEnvironmentVariable(const char *name)
{
	char **env_pointer;
	int len = lengthOfList(environ, (char *)name);

	if (len == -1)
	{
		write(STDERR_FILENO, "unsetenv: variable not found\n", 29);
		status = 2;
		return (DONT_CREATE_NEW_CHILD_PROCESS);
	}

	env_pointer = environ + len;
	free(*env_pointer);
	while (*(env_pointer + 1) != NULL)
	{
		*env_pointer = *(env_pointer + 1);
		env_pointer++;
	}
	*env_pointer = NULL;
	status = 0;

	return (DONT_CREATE_NEW_CHILD_PROCESS);
}
