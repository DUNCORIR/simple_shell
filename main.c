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
	char *lineptr = NULL;
	size_t len = 0;
	ssize_t	nread = 0;
	int line_number = 1;

	(void)argc;

	while (1)
	{
		display_prompt();  /* Helper function to display the prompt */

		nread = custom_getline(&lineptr, &len);
		if (nread == -1) /* Handle EOF or error */
		{
			free(lineptr); /* Free memory on EOF */
			exit(EXIT_SUCCESS); /* Exit normally */
		}
		if (nread > 0 && lineptr[nread - 1] == '\n') /* Rm newline character  */
			lineptr[nread - 1] = '\0';
		handle_input(lineptr, argv, line_number);  /* Process input */
		line_number++;  /* Increment line number for each input */
	}
	free(lineptr);  /* Free input buffer before exiting */
	return (EXIT_SUCCESS);
}
/* Helper function to display the shell prompt */

/**
 * display_prompt - Displays the shell prompt to the user.
 * This function checks if the input is from a terminal (interactive mode)
 * and, if true, displays the shell prompt (`$`) to indicate readiness
 * for input. It flushes the output buffer after printing.
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))  /* Check if input is from terminal */
	{
		printf("$ ");
		fflush(stdout);
	}
}

/* Helper function to free parsed arguments */

/**
 * free_args - Frees the memory allocated for parsed arguments.
 * @args: The array of arguments to be freed.
 *
 * This function loops through the array of arguments and frees each one,
 * followed by freeing the array itself.
 */
void free_args(char **args)
{
	int i;

	for (i = 0; args[i] != NULL; i++)
	{
		free(args[i]);
	}
	free(args);
}
