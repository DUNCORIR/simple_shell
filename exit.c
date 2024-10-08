#include "shell.h"

/**
 * execute_exit - Exits the shell.
 * @args: Array of arguments, with optional exit status.
 * @last_status: The status code of the last executed command.
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
		errno = 0;  /* Reset errno before conversion */
		status = strtol(args[1], &endptr, 10);

		if (*endptr != '\0' || errno != 0 || status < 0) /* Check conversion success */
		{
			fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n", args[1]);
			for (i = 0; args[i] != NULL; i++)
			{
				free(args[i]);
			}
			free(args);
			exit(2);
		}
		status = status % 256;
	}
	else
	{
		status = last_status % 256; /* Last command's status if no argument given */
	}
	/* Free each argument string (if they were dynamically allocated) */
	for (i = 0; args[i] != NULL; i++)
	{
		free(args[i]);
	}
	free(args);
	exit(status); /* Exit with the status code */
}
