#include "shell.h"

/**
 * execute_exit - Exits the shell.
 * @args: Array of arguments, with optional exit status.
 *
 * Description: The function handles "exit" command shell.
 * if Exit command is followed by status argument,
 * it exits with that status. If no status is provided
 * it exits with the status of the last
 *
 */
void execute_exit(char **args)
{
	/* Default exit status if no argument is provided */
	int status = 0;

	if (args[1] != NULL)
	{
		/* Convert the argument to an integer status code */
		char *endptr; 
		status = strtol(args[1], &endptr, 10);

		if (*endptr != '\0')
		{
			fprintf(stderr, "Invalid exit status: %s\n", args[1]);
			return; /* Return without exiting if status is invalid */
		}
	}
	/*  Exit the shell with the provided or default status */
	exit(status);
}
