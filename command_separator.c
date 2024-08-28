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
			/* Check for '&&' and '||' at the beginning of the command */
			char *subcommand;

			subcommand = custom_strtok(trimmed_command, "&&"); /* Handle '&&' */
			while (subcommand != NULL)
			{
				char *subsubcommand = custom_strtok(subcommand, "||"); /* Handle '||' */
				while (subsubcommand != NULL)
				{
					char **parsed_args = parse_input(subsubcommand);

					if (parsed_args[0] != NULL)
					{
						if (last_status == 0)
						{

							last_status = execute_command(parsed_args, environ, program_name, line_number);
						}
					}
					free(parsed_args); /* Free the argument list */
					subsubcommand = custom_strtok(NULL, "||");
				}
				subcommand = custom_strtok(NULL, "&&");
			}
		}
		command = custom_strtok(NULL, delim); /* Get the next command */
	}

	free(input_copy); /* Free duplicated input */
}
