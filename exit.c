#include "shell.h"

/**
 * execute_exit - Exits the shell.
 * @args: Array of arguments, with optional exit status.
 *
 * Description: The function handles "exit" command shell.
 *
 */
void execute_exit(__attribute__((unused)) char **args)
{
	exit(EXIT_SUCCESS);
}
