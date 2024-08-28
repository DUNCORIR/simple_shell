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
void execute_exit( __attribute__((unused)) char **args)
{
	exit(EXIT_SUCCESS);
}
