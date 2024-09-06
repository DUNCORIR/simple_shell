#include "shell.h"

/**
 * execute_exit - Exits the shell.
 * @args: Array of arguments, with optional exit status.
 *
 * Description: The function handles "exit" command shell.
 *
 */
void execute_exit(char **args, int last_status)
{
	int status; 
	char *endptr;

	if (args[1] != NULL)
	{
		status = strtol(args[1], &endptr, 10);

		if (*endptr != '\0') /* Check conversion success */
		{
			fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n", args[1]);
			status = 2;
		}
		else
		{
			status = last_status % 256; /*last command's status if no argument is provided */
		}
	}
	else
	{
		status = last_status; /* Last command's status if no argument is provided */
	}
	free(args);
	exit(status); /* Exit with the status code */
}
