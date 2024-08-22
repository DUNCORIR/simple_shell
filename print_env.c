#include "shell.h"

/**
 * print_env - Prints the current env variables.
 * @envp: Array of strings representing environment variables.
 * environment vector.
 *
 * Description: Iterates through environment variables and
 * prints each variable to standard output.
 *
 * Return: Nothing.
 */
void print_env(char **envp)
{
	while (*envp != NULL)
	{
		printf("%s\n", *envp);
		envp++;
	}
}
