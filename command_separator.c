#include "shell.h"

/**
 * execute_commands - Execute multiple commands separated by semicolons.
 * @input: The input string containing commands separated by semicolons.
 * @environ: The environment variables.
 *
 * Return: void
 */
void execute_commands(char *input, char **environ, char *program_name, int line_number)
{
	char *command;
	int last_status = 0;
	char *input_copy = custom_strdup(input);
	char *delim = ";";
	char **parsed_args = NULL;

	if (input_copy == NULL)
	{
		perror("custom_strdup");
		exit(EXIT_FAILURE);
	}

	command = custom_strtok(input_copy, delim); /* Split commands by ';' */
	while (command != NULL)
	{
		char *trimmed_command = custom_strtok(command, "\n");
		int should_execute = 1;

		if (trimmed_command != NULL && *trimmed_command != '\0')
		{
			/* Check for '&&' and '||' at the beginning of the command */
			if (strncmp(trimmed_command, "&&", 2) == 0)
			{
				if (last_status != 0)
					should_execute = 0; /* Skip if the last command failed */
				trimmed_command += 2;  /* Move past '&&' */
			}
			else if (strncmp(trimmed_command, "||", 2) == 0)
			{
				if (last_status == 0)
					should_execute = 0; /* Skip if the last command succeeded */
				trimmed_command += 2;  /* Move past '||' */
			}
			if (should_execute && *trimmed_command != '\0')
			{
				char **parsed_args = parse_input(trimmed_command);
				if (parsed_args[0] != NULL)
				{
					int builtin_status = handle_builtins(parsed_args, environ, program_name, line_number);
					if (!builtin_status)
						last_status = execute_command(parsed_args, environ, program_name, line_number);
				}
			}
			free(parsed_args); /* Free the argument list */
		}
		command = custom_strtok(NULL, delim); /* Get the next command */
	}
	free(input_copy); /* Free duplicated input */
}
