#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define NEGATIVE 0
#define POSITIVE 1
#define NONE 2
#define EQUAL 3
#define PREFIX 4
#define TERMINATE_PROCESS_EXECUTION 5
#define DONT_CREATE_NEW_CHILD_PROCESS 6
#define LAUNCH_RUNTIME_PROCESS 7

extern char *nameOfShell;
extern int status;
extern int lineNumber;
extern char **environ;

/**
 * struct Alias - aliases linked list
 * @nodeName: alias name
 * @nodeValue: command called by alias
 * @nextNode: points to nextNode node
 */

typedef struct Alias
{
    char *nodeName;
    char *nodeValue;
    struct Alias *nextNode;
} alias;

void displayErrorMessage(char *arg0, char *arg1);
char *checkIfNotBuiltIn(char **arguements);
char *inputCleaner(char *previousBuffer, size_t *previousSize);
char *checkIfArgumentIsVariable(char *arguement);
char *_itoa(int integer);
char *copyString(char *source);
char *concatenateString(char *firstString, char *secondString);
char *findArrayElement(char **array, char *itemName);
char **createArray(char *string, char endOfListEntryChar, char **ifSemiColonPointer);
char **duplicateArray(char **previousArray, int createdArraySize);
int releaseArray(char **arguements);
int configureEnvironmentVariable(const char *name, const char *value);
int runCommand(char **arguements);
int stringToInteger(char *string);
int errorCheckerForInput(char *pointer);
int getStringLength(char *string);
int lengthOfList(char **testList, char *indexedEntry);
int compareString(char *firstString, char *secondString, int pref_or_match);
int deleteEnvironmentVariable(const char *name);
int commandRuntimeHandler(char **arguements);
int checkIfCommandInbuilt(char **arguements);
int logicalOperatorHandler(char **arguements, char operator, int last_compare);
int changeDirectory(char *name);
int commandAliasProcessing(char **arguements, int free);
int releaseAlias(alias *aliasPointer);
int replaceIfAlias(char **arguements, alias *aliasPointer);
int displayAliases(alias *aliasPointer);
int displayAliasValue(char *arguement, alias *aliasPointer);
int configureAlias(char *arguement, alias *aliasPointer, char *setValue);
int displayEnvironMentVariable(void);

#endif
