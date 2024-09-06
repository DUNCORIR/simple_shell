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
		nread = custom_getline(&lineptr, &len);
		if (nread == -1) /* Handle EOF or error */
		{
			if (lineptr != NULL)
			{
				free(lineptr); /* Free memory on EOF */
				lineptr = NULL;
			}
			exit(EXIT_SUCCESS); /* Exit normally */
		}
		if (nread > 0 && lineptr[nread - 1] == '\n') /* Remove newline character if present */
		{
			lineptr[nread - 1] = '\0';
		}
		args = parse_input(lineptr, last_status); /* Parse the input into arguments */
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
	free(lineptr); /* Free the input buffer before exiting */
	return (EXIT_SUCCESS);
}

void handle_logical_operators(char *input, char **argv,
		int *last_status, int *line_number)
{
	char *token, *rest = input;
	int logical_operator = 0; /* 0: None, 1: &&, 2: */
	(void)argv;

	while ((token = custom_strtok(rest, " \t")) != NULL)
	{
		if (strcmp(token, "&&") == 0)
		{
			logical_operator = 1;
			rest = NULL;
			continue;
		}
		else if (strcmp(token, "||") == 0)
		{
			logical_operator = 2;
			rest = NULL; /* Continue to next token */
			continue;
		}
		/* Execute the command */
		 execute_commands(token, argv, argv[0], *line_number);
		/* Handle logical operators */
		if (logical_operator == 1 && *last_status != 0)
		{
			/*Skip the next command if the last command failed */
			while ((token = custom_strtok(rest, " \t")) != NULL)
			{
				if (strcmp(token, "&&") == 0 || strcmp(token, "||") == 0)
				{
					logical_operator = 0; /* reset operator */
					rest = NULL;
					break;
				}
				rest = NULL; /* Continue to next token */
			}
		}
		else if (logical_operator == 2 && *last_status == 0)
		{
			/* Skip the next command if the last command succeeded*/
			while ((token = custom_strtok(rest, " \t")) != NULL)
			{
				if (strcmp(token, "&&") == 0 || strcmp(token, "||") == 0)
				{
					logical_operator = 0;
					rest = NULL;
					break;
				}
				rest = NULL; /* Continue to next token */
			}
		}
		logical_operator = 0; /* reset logical operator */
	}
}
				
