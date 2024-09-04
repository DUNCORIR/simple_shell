#include "shell.h"

/**
 * main - Entry point for the shell program.
 * @argc: The number of command-line arguments.
 * @argv: An array of command-line arguments.
 *
 * Description:Initializes the shell, displays the prompt, reads user input,
 *		and executes the corresponding command or built-in function
 *		in a loop until an exit condition is met.
 *
 * Return: 0 on success, or another status code on failure
 */
int main(int argc, char **argv)
{
	char *input = NULL;
	size_t len = 0;
	ssize_t	nread;
	char **args;
	int line_number = 1, last_status = 0, i;

	(void)argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			printf("$ "); /* Display prompt */
			fflush(stdout);
		} 
		nread = custom_getline(&input, &len);
		if (nread == -1) /* Handle EOF or error */
		{
			if (input != NULL)
			{
				free(input); /* Free memory on EOF */
			}
			exit(EXIT_SUCCESS); /* Exit normally */
		}
		if (nread > 0 && input[nread - 1] == '\n') /* Remove newline character if present */
		{
			input[nread - 1] = '\0';
		}
		args = parse_input(input); /* Parse the input into arguments */
		if (args && args[0] != NULL) /* Ensure args are valid */
		{
			last_status = execute_command(args, environ,
					argv[0], line_number, last_status);
				for (i = 0; args[i] != NULL; i++)
				{
					free(args[i]);
				}
				free(args); /* Free the args array */
		}
		line_number++; /* Increment line number for each input */
	}
	free(input); /* Free the input buffer before exiting */
	return (EXIT_SUCCESS);
}
