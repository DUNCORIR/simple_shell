#include "shell.h"

/**
 * print_env - Prints the current env variables.
 * envp: Array of environment variables.
 *
 * Description: Iterates through environment variables and
 * prints each variable on new line.
 */
void print_env(char **envp)
{
	while (*envp != NULL)
	{
		printf("%s\n", *envp);
		envp++;
	}
}
