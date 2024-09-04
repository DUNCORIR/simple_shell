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
	int i;

	if (args[1] != NULL)
	{
		status = strtol(args[1], &endptr, 10);

		if (*endptr != '\0') /* Check conversion success */
		{
			fprintf(stderr, "%s: exit: %s: numeric argument required\n",
					args[0], args[1]);
			status = 2;
		}
		else
		{
			status = status % 256;
		}
	}
	else
	{
		status = last_status;
	}
	for (i = 0; args[i] != NULL; i++) /* No arg,, use the last command's status */
	{
		free(args[i]);
	}
	free(args);
	exit(status); /* Exit with the status code */
}
