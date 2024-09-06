#include "shell.h"

/**
 * handle_input - Handles reading input from the user.
 * @input: Pointer to the input string.
 * @len: Pointer to the size of the buffer for getline.
 * @nread: Pointer to store the number of bytes read by getline.
 * @argv: Array of arguments
 * @line_number: The current line number being processed.
 *
 * Description: This function reads the user's input using getline,
 * checks for errors, and handles the EOF condition.
 */
void handle_input(char **input, size_t *len,
		ssize_t *nread, char **argv, int line_number)
{
	char *equal_sign;
	int last_status = 0;

	*nread = custom_getline(input, len);/* Read input from user */
	if (*nread == -1) /* Check for errors or EOF */
	{
		if (*nread == 0) /* EOF */
		{
			free(*input); /* Free the allocated memory for input */
			exit(EXIT_SUCCESS); /* Exit normally on EOF */
		}
		perror("custom_getline"); /* Print error message if getline fails */
		free(*input); /* Free memory on error */
		exit(EXIT_FAILURE); /* Exit with failure on error */
	}
	if (*nread > 0 && (*input)[*nread - 1] == '\n')
	{
		(*input)[*nread - 1] = '\0'; /* Replace newline */
		(*nread)--; /* Adjust length */
	}

	equal_sign = strchr(*input, '=');/* Handle environment variable assignment */
	if (equal_sign != NULL)
	{
		handle_env_assignment(*input);
		return; /* Skip command execution */
	}
	/* Handle command separators and execute commands */
	execute_commands_with_separator(*input, argv, line_number, last_status);
}

/**
 * handle_env_assignment - Handles the assignment of environment variables.
 *  @input: The input string containing the variable and its value.
 *
 * Description: This function parses the input string to
 * separate the variable name and its value.sets the environment
 * variable using setenv.
 */
void handle_env_assignment(char *input)
{
	char *var = custom_strtok(input, "=");
	char *val = custom_strtok(NULL, "=");

	if (var && val)
	{
		if (setenv(var, val, 1) == -1)
			perror("setenv"); /* Handle setenv error */
	}
	else
		fprintf(stderr, "Invalid environment variable assignment\n");
}

/**
 * execute_commands_with_separator - Executes commands separated by ';'.
 * @input: The input string containing commands separated by ';'.
 * @argv: The argument vector of the shell.
 * @line_number: The current line number being executed.
 *
 * Description: This function splits the input string into
 * individual commands based on the command separator ';'.
 * It then parses and executes each command.
 * Any errors during parsing or execution are reported.
 *
 */

void execute_commands_with_separator(char *input, char **argv,
		int line_number, int last_status)
{
	char **commands = parse_commands(input);

	int i;

	if (commands)
	{
		for (i = 0; commands[i]; i++)
		{
			char **args = parse_input(commands[i], last_status);

			if (args)
			{
				execute_command_or_builtin(args, environ, argv[0], line_number);
				free(args); /* Free parsed arguments */
			}
			else
				fprintf(stderr, "Error parsing command: %s\n", commands[i]);
			free(commands[i]); /* Free each command string */
		}
		free(commands); /* Free commands array */
	}
}


/**
 * handle_builtin - Handles the execution of built-in shell commands.
 * @args: An array of arguments passed to the command.
 * @environ: An array of environment variables.
 * @program_name: The name of the shell program (argv[0]).
 * @line_number: The line number in the script or input being executed.
 *
 * Return: 1 if a built-in command was executed, 0 otherwise.
 */

int handle_builtin(char **args, char **environ,
		__attribute__((unused)) char *program_name,
		__attribute__((unused)) int line_number)
{
	int last_status = 0;

	if (strcmp(args[0], "exit") == 0)
	{
		execute_exit(args, last_status);
		return (1); /* Indicate that a built-in command was handled */
	}
	if (strcmp(args[0], "cd") == 0)
	{
		return (execute_cd(args)); /* Return the result of execute_cd */
	}
	if (strcmp(args[0], "env") == 0)
	{
		print_env(environ); /* handle built in function */
		return (1);
	}
	if (strcmp(args[0], "setenv") == 0)
	{
		execute_setenv(args); /* Handle setenv built-in function */
		return (1);
	}
	if (strcmp(args[0], "unsetenv") == 0)
	{
		execute_unsetenv(args); /*Handle unsetenv built-in function */
		return (1);
	}
	return (0);
}
