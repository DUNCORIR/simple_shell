#include "shell.h"

/**
 * execute_exit - Exits the shell.
 * @args: Array of arguments, with optional exit status.
 *
 * Description: The function handles "exit" command shell.
 *
 */
void execute_exit(char **args)
{
	int status = 0;

	if (args[1] != NULL)
	{
		char *endptr;

		status = strtol(args[1], &endptr, 10);

		if (*endptr != '\0') /* Check conversion success */
		{
			fprintf(stderr, "%s: exit: %s: numeric argument required\n",
					args[0], args[1]);
			exit(2); /* Exit with status 2 for invalid numeric argument */
		}
	}

	exit(status); /* Exit with the status code */
}
