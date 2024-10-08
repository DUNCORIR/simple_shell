#include "shell.h"

/**
 * execute_commands - Execute multiple commands separated by semicolons.
 * @input: The input string containing commands separated by semicolons.
 * @environ: The environment variables
 * @program_name: The name of the shell
 *	program (for error messages).
 * @line_number: The line number of the command in the
 *	input (for error messages)..
 *
 * Return: void
 */
void execute_commands(char *input, char **environ,
		char *program_name, int line_number)
{
	char *command;
	char *input_copy = custom_strdup(input);
	char *delim = ";";

	if (input_copy == NULL)
	{
		perror("custom_strdup");
		exit(EXIT_FAILURE);
	}

	command = custom_strtok(input_copy, delim); /* Split commands by ';' */
	while (command != NULL)
	{
		char *trimmed_command = custom_strtok(command, "\n");

		if (trimmed_command != NULL && *trimmed_command != '\0')
		{
			handle_subcommands(trimmed_command, environ, program_name, line_number);
		}
		command = custom_strtok(NULL, delim); /* Get the next command */
	}
	free(input_copy); /* Free duplicated input */
}

/**
 * handle_subcommands - Handles subcommands separated by '&&' and '||'.
 * @trimmed_command: The command to be handled.
 * @environ: The environment variables to be used.
 * @program_name: The name of the shell program (for error messages).
 * @line_number: The line number of the command in the
 *	input (for error messages).
 *
 * Return: None
 */
void handle_subcommands(char *trimmed_command, char **environ,
		char *program_name, int line_number)
{
	char *subcommand;
	int last_status = 0;
	char *trimmed_subcommand;

	subcommand = custom_strtok(trimmed_command, "&&"); /* Handle '&&' */
	while (subcommand != NULL)
	{
		trimmed_subcommand = custom_strtok(subcommand, "||");

		while (trimmed_subcommand != NULL)
		{
			char **parsed_args = parse_input(trimmed_subcommand, last_status);

			if (parsed_args[0] != NULL)
			{
				if (last_status == 0)
				{
					last_status = execute_command(parsed_args,
							environ, program_name, line_number, last_status);
				}
			}
			free(parsed_args); /* Free the argument list */
			trimmed_subcommand = custom_strtok(NULL, "||");
		}
		subcommand = custom_strtok(NULL, "&&");
	}
}

/**
 * int_to_string - Converts an integer to a string.
 * @num: The integer to convert.
 *
 * Return: The string representation of the integer or NULL on failure.
 */
char *int_to_string(int num)
{
	char *str;
	int len = snprintf(NULL, 0, "%d", num);

	str = malloc(len + 1);
	if (str)
		snprintf(str, len + 1, "%d", num);
	return (str);
}

/**
 * handle_allocation_failure - Handles memory allocation
 * failure by freeing allocated memory.
 * @commands: The commands array to free.
 * @count: The number of allocated commands to free.
 */
void handle_allocation_failure(char **commands, size_t count)
{
	while (count > 0)
		free(commands[--count]);
	free(commands);
}
