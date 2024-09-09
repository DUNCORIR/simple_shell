#include "shell.h"

/**
 * custom_strdup - Custom implementation of strdup.
 * @str: The input string to duplicate.
 *
 * Return: A pointer to the duplicated string, or NULL on failure.
 *
 * Description: This function allocates memory and duplicates
 *	the given string. It returns a pointer to the
 *	newly allocated string, or NULL if the allocation
 *	fails.
 */
char *custom_strdup(const char *str)
{
	char *dup_str;
	size_t len, i;

	/* check NULL input */
	if (str == NULL)
		return (NULL);

	/* Get the length of input string */
	len = strlen(str) + 1; /* +1 for the null terminator */

	/* Allocate input string to the allocated memory */
	dup_str = malloc(len * sizeof(char));
	if (dup_str == NULL)
		return (NULL); /* Handle memory allocation failure */

	for (i = 0; i < len; i++)
	{
		dup_str[i] = str[i];
	}
	dup_str[len - 1] = '\0'; /* Copy the string */
	return (dup_str);
}

/**
 * parse_commands - Splits input into commands based on ';'
 * @input: The input string.
 *
 * Return: An array of command strings.
 */
char **parse_commands(char *input)
{
	char **commands, **new_commands;
	char *token;
	size_t bufsize = INIT_BUF_SIZE, i = 0;

	/* Estimate the number of commands and allocate memory */
	commands = malloc(sizeof(char *)); /* memory */
	if (!commands)
		return (NULL);
	token = custom_strtok(input, ";"); /* Split to commands by custom_strtok */
	while (token != NULL)
	{
		char *subtoken; /* Split further by && and || */

		subtoken = custom_strtok(token, " \t"); /* Split by spaces/tabs */
		while (subtoken != NULL)
		{
			if (i >= bufsize) /* Resize the commands array if needed */
			{
				bufsize *= 2;
				new_commands = realloc(commands, bufsize * sizeof(char *));
				if (!new_commands)
				{
					handle_allocation_failure(commands, i);
					return (NULL);
				}
				commands = new_commands;
			}
			commands[i] = custom_strdup(subtoken);
			if (!commands[i])
			{
				handle_allocation_failure(commands, i);
				return (NULL);
			}
			i++;
			subtoken = custom_strtok(NULL, " \t");
		}
		token = custom_strtok(NULL, ";");
	}
	commands[i] = NULL;
	return (commands);
}

/**
 * parse_input - Parses the input string into an
 * array of arguments, handling variable replacement.
 * @input: The input string containing arguments
 * separated by spaces and tabs.
 * @last_status: The status code of the last executed command.
 *
 * Return: An array of arguments or NULL on failure.
 */
char **parse_input(char *input, int last_status)
{
	char **new_args, **args, *token, *replaced_token;
	size_t bufsize = INIT_BUF_SIZE, position = 0;

	args = malloc(bufsize * sizeof(char *));
	if (!args)
		return (NULL);

	token = custom_strtok(input, " \t");
	while (token != NULL)
	{
		if (position >= bufsize)
		{
			bufsize *= 2;
			new_args = realloc(args, bufsize * sizeof(char *));

			if (!new_args)
			{
				handle_allocation_failure(args, position);
				return (NULL);
			}
			args = new_args;
		}
		replaced_token = handle_variable_replacement(token, last_status);
		if (!replaced_token)
		{
			handle_allocation_failure(args, position);
			return (NULL);
		}
		args[position] = replaced_token;
		position++;
		token = custom_strtok(NULL, " \t");
	}
	args[position] = NULL;  /* Null-terminate the array */
	return (args);
}

/**
 * handle_variable_replacement - Replaces variables
 * with their corresponding values.
 * @token: The token to check for variable replacement.
 * @last_status: The status code of the last executed command.
 *
 * Return: A string with variables replaced or NULL on failure.
 */
char *handle_variable_replacement(const char *token, int last_status)
{
	char *replaced_token;

	if (strcmp(token, "$?") == 0)
		replaced_token = int_to_string(last_status);
	else if (strcmp(token, "$$") == 0)
		replaced_token = int_to_string(getpid());
	else
		replaced_token = custom_strdup(token);

	return (replaced_token);
}

/**
 * handle_external_command - Handles the execution of external commands.
 * @args: An array of arguments passed to the command.
 * @environ: An array of environment variables.
 * @program_name: The name of the shell program (argv[0]).
 * @line_number: The line number in the script or input being executed
 */
void handle_external_command(char **args, char **environ,
		char *program_name, int line_number)
{
	int last_status = 0;

	char *command_path = search_path(args[0]);

	if (command_path != NULL)
	{
		args[0] = command_path;
		execute_command(args, environ, program_name, line_number, last_status);
		free(command_path);
	}
	else
	{
		fprintf(stderr, "%s: %d: %s: not found\n", program_name,
				line_number, args[0]);
	}
}
