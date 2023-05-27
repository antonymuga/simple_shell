#include "shell.h"

int status;

char *nameOfShell;

/**
 * checkIfNotBuiltIn - tests for non-built-in commands
 * @arguements: commands
 * Return: POSITIVE for command, NEGATIVE if not
 */

char *checkIfNotBuiltIn(char **arguements)
{
	char *commandBuffer;
	char *fullBuffer;
	char *environmentPathString = NULL;
	char *environmentPathPointer;
	char *temporaryEnvironmentPath;
	char **environmentPathVariable;
	char **environmentPathVariablePointer;

	if (access(*arguements, X_OK) == 0)
		return (copyString(*arguements));

	if (findArrayElement(environ, "PATH") != NULL)
		environmentPathString = copyString(findArrayElement(environ, "PATH") + 5);

	environmentPathPointer = environmentPathString;

	if (environmentPathString != NULL)
	{
		if (*environmentPathString == ':')
		{
			commandBuffer = concatenateString("./", *arguements);
			if (access(commandBuffer, X_OK) == 0)
			{
				free(environmentPathString);
				return (commandBuffer);
			}
			else
			{
				free(commandBuffer);
				environmentPathPointer = copyString(environmentPathString + 1);
				free(environmentPathString);
				environmentPathString = environmentPathPointer;
			}
		}
		while (*environmentPathPointer != '\0')
		{
			if (*environmentPathPointer == ':' && *(environmentPathPointer + 1) == ':')
			{
				*(environmentPathPointer + 1) = '\0';
				temporaryEnvironmentPath = (concatenateString(environmentPathString, ".:"));
				environmentPathPointer = concatenateString(temporaryEnvironmentPath, environmentPathPointer + 2);
				free(environmentPathString);
				free(temporaryEnvironmentPath);
				environmentPathString = environmentPathPointer;
			}
			environmentPathPointer++;
		}
	}

	environmentPathVariable = createArray(environmentPathString, ':', NULL);
	environmentPathVariablePointer = environmentPathVariable;

	commandBuffer = concatenateString("/", *arguements);

	fullBuffer = copyString(commandBuffer);

	while (*environmentPathVariablePointer != NULL && access(fullBuffer, X_OK) != 0)
	{
		free(fullBuffer);
		fullBuffer = concatenateString(*environmentPathVariablePointer, commandBuffer);
		environmentPathVariablePointer++;
	}

	free(commandBuffer);
	free(environmentPathString);
	free(environmentPathVariable);

	if (access(fullBuffer, X_OK) != 0)
	{
		displayErrorMessage(arguements[0], NULL);
		free(fullBuffer);
		return (NULL);
	}

	return (fullBuffer);
}


/**
 * commandRuntimeHandler - manages runtime process of a command
 * @arguements: commands
 * Return: POSITIVE for success, NEGATIVE if there's an error
 */

int commandRuntimeHandler(char **arguements)
{
	char **arguementsPointer = arguements;
	int lastEvaluation = NONE;
	int nullError = POSITIVE;
	char lastOperationCode = 'c';
	char nextOperationCode = 'c';
	int nextStep;

	while (*arguements != NULL && lastEvaluation != TERMINATE_PROCESS_EXECUTION)
	{
		while (*arguementsPointer != NULL && **arguementsPointer != '&'
		       && **arguementsPointer != '|')
			arguementsPointer++;

		if (compareString(*arguementsPointer, "||", EQUAL) == POSITIVE)
		{
			*arguementsPointer = NULL;
			arguementsPointer++;
			nextOperationCode = '|';
		}
		if (compareString(*arguementsPointer, "&&", EQUAL) == POSITIVE)
		{
			*arguementsPointer = NULL;
			arguementsPointer++;
			nextOperationCode = '&';
		}
		if (nextOperationCode == 'c')
			break;

		lastEvaluation = logicalOperatorHandler(arguements, lastOperationCode, lastEvaluation);
		if (lastEvaluation == NEGATIVE)
			nullError = NEGATIVE;
		lastOperationCode = nextOperationCode;
		arguements = arguementsPointer;
	}

	if (nextOperationCode == 'c')
	{
		nextStep = runCommand(arguements);

		if (nextStep == TERMINATE_PROCESS_EXECUTION)
			return (TERMINATE_PROCESS_EXECUTION);
	}

	if (nullError == NEGATIVE || nextStep == NEGATIVE)
		return (NEGATIVE);

	return (POSITIVE);
}

/**
 * changeDirectory - change directory logic
 * @name: directory name
 * Return: Status code 0 if successful
 */

int changeDirectory(char *name)
{
	char *home;
	char *pwd;
	char path_buffer[PATH_MAX];
	size_t sizeOfBuffer = PATH_MAX;
	int i;

	getcwd(path_buffer, sizeOfBuffer);

	if (name == NULL || compareString("~", name, PREFIX) == POSITIVE
	    || compareString("$HOME", name, EQUAL) == POSITIVE)
	{
		if (name != NULL && *name == '~' && *(name + 1) != '\0'
		    && *(name + 1) != '/')
		{
			status = 2;
			displayErrorMessage("cd", name);
			return (DONT_CREATE_NEW_CHILD_PROCESS);
		}

		home = findArrayElement(environ, "HOME");
		if (home == NULL)
		{
			status = 2;
			displayErrorMessage("cd", name);
			return (DONT_CREATE_NEW_CHILD_PROCESS);
		}

		while (*home != '=')
			home++;

		home++;
		i = chdir((const char *)home);
		if (i != -1)
			configureEnvironmentVariable("PWD", (const char *)home);

		if (name != NULL && compareString("~/", name, PREFIX) == POSITIVE)
			name += 2;
	}
	if (compareString("-", name, EQUAL) == POSITIVE)
	{
		pwd = findArrayElement(environ, "OLDPWD");
		if (pwd == NULL)
			return (2);

		while (*pwd != '=')
		{
			pwd++;
		}
		pwd++;

		i = chdir((const char *)pwd);
		if (i != -1)
		{
			write(STDOUT_FILENO, pwd, getStringLength(pwd));
			write(STDOUT_FILENO, "\n", 1);
			configureEnvironmentVariable("PWD", (const char *)pwd);
		}
	}
	if (name != NULL && compareString("~", name, EQUAL) == NEGATIVE
	    && compareString("$HOME", name, EQUAL) == NEGATIVE && i != -1
	    && *name != '\0' && compareString("-", name, EQUAL) != POSITIVE)
	{
		i = chdir((const char *)name);
		if (i != -1)
			configureEnvironmentVariable("PWD", (const char *)name);
	}
	if (i == -1)
	{
		status = 2;
		displayErrorMessage("cd", name);
		return (DONT_CREATE_NEW_CHILD_PROCESS);
	}

	status = 0;
	configureEnvironmentVariable("OLDPWD", (const char *)path_buffer);

	return (DONT_CREATE_NEW_CHILD_PROCESS);
}


/**
 * runCommand - runs the command
 * @arguements: commands
 * Return: POSITIVE or TERMINATE_PROCESS_EXECUTION
 */

int runCommand(char **arguements)
{
	char *bufferPointer = *arguements;
	char *command_name;
	pid_t process_id;
	int nextStep = checkIfCommandInbuilt(arguements);

	if (nextStep == LAUNCH_RUNTIME_PROCESS)
	{
		command_name = checkIfNotBuiltIn(arguements);
		if (command_name == NULL)
			return (NEGATIVE);

		process_id = fork();
		if (process_id == -1)
		{
			exit(EXIT_FAILURE);
		}
		if (process_id == 0)
		{
			execve(command_name, arguements, environ);
			exit(EXIT_FAILURE);
		}
		wait(&status);
		free(command_name);
		fflush(stdin);
	}

	if (compareString("NEGATIVE", *arguements, EQUAL) == POSITIVE)
		status = 1;

	if (*arguements != bufferPointer)
		free(*arguements);
	arguements++;

	while (*arguements != NULL)
	{
		while (*bufferPointer != '\0')
		{
			bufferPointer++;
		}
		bufferPointer++;

		if (*arguements != bufferPointer)
			free(*arguements);

		arguements++;
	}

	if (nextStep == TERMINATE_PROCESS_EXECUTION)
		return (TERMINATE_PROCESS_EXECUTION);

	if (status != 0)
		return (NEGATIVE);

	return (POSITIVE);
}
