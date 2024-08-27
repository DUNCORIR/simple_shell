#include "shell.h"

/**
 *  * execute_setenv - Initializes or modifies an environment variable
 *  @args: Array of arguments (args[1] is the variable name, 
 *	args[2] is the value).
 *
 * Return: Nothing.
 */
void execute_setenv(char **args)
{
	if (args[1] == NULL || args[2] == NULL)
	{
		fprintf(stderr, "setenv: Usage: setenv VARIABLE VALUE\n");
		return;
	}

	if (setenv(args[1], args[2], 1) == -1)
	{
		perror("setenv");
	}
}
