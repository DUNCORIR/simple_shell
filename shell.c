#include "shell.h"

/**
 * handle_input - Handles reading input from the user.
 * @lineptr: The input string to process.
 * @argv: Array of arguments
 * @line_number: The current line number being processed.
 *
 * Description: This function reads the user's input using getline,
 * checks for errors, and handles the EOF condition.
 */
void handle_input(char *lineptr, char **argv, int line_number)
{
	char *equal_sign;
	int last_status = 0;
	char **args;


	if (lineptr == NULL)
		return;

	equal_sign = strchr(lineptr, '=');/* Handle environment variable assignment */
	if (equal_sign != NULL)
	{
		handle_env_assignment(lineptr);
		return; /* Skip command execution */
	}
	args = parse_input(lineptr, last_status);

	if (args && args[0] != NULL) /* Execute command if args are valid */
	{
		last_status = execute_command(args,
				environ, argv[0], line_number, last_status);
		free_args(args); /* Free the argument array */
	}
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
 * @last_status: The status code of the last executed command.
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
				last_status = execute_command_or_builtin(args,
						environ, argv[0], line_number);
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
