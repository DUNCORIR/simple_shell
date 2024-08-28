#include "shell.h"

/**
 * execute_unsetenv - Removes an environment variable.
 * @args: Array of arguments (args[1]
 *	is the variable name to remove).
 *
 * Return: Nothing.
 */
void execute_unsetenv(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "unsetenv: Usage: unsetenv VARIABLE\n");
		return;
	}

	if (unsetenv(args[1]) == -1)
	{
		perror("unsetenv");
	}
}
