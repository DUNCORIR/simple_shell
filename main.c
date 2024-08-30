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
	int line_number = 1;

	(void)argc;

	while (1)
	{
		printf("$ "); /* Display prompt */
		fflush(stdout);

		nread = custom_getline(&input, &len);
		if (nread == -1) /* Handle EOF or error */
		{
			if (nread == 0 || nread == -1)
			{
				free(input); /* Free memory on EOF */
				exit(EXIT_SUCCESS); /* Exit normally */
			}
			perror("custom_getline"); /* Print error if getline fails */
			free(input); /* Free memory on error */
			exit(EXIT_FAILURE); /* Exit with failure on error */
		}
		/* Remove newline character if present */
		if (nread > 0 && input[nread - 1] == '\n')
		{
			input[nread - 1] = '\0';
		}
		args = parse_input(input); /* Parse the input into arguments */
		if (args && args[0] != NULL) /* Ensure args are valid */
		{
			execute_command(args, environ, argv[0], line_number);
		}
		free(args); /* Free parsed arguments */
		line_number++; /* Increment line number for each input */
	}
	free(input); /* Free the input string memory */
	return (EXIT_SUCCESS);
}
