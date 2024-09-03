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
	size_t len;
	size_t i;

	/* check NULL input */
	if (str == NULL)
		return (NULL);

	/* Get the length of input string */
	len = strlen(str);

	/* Allocate input string to the allocated memory */
	dup_str = malloc((len + 1) * sizeof(char));
	if (dup_str == NULL)
		return (NULL); /* Handle memory allocation failure */

	for (i = 0; i < len; i++)
	{
		dup_str[i] = str[i];
	}
	dup_str[len] = '\0';
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
	char **commands;
	char *token;
	int i = 0;

	/* Estimate the number of commands and allocate memory */
	commands = malloc(sizeof(char *) * (strlen(input) / 2 + 2)); /* memory */
	if (!commands)
		return (NULL);

	/* Split the input into commands using custom_strtok */
	token = custom_strtok(input, ";");
	while (token != NULL)
	{
		char *subtoken; /* Split further by && and || */

		subtoken = custom_strtok(token, " \t");
		while (subtoken != NULL)
		{
			commands[i] = custom_strdup(subtoken);
			if (!commands[i])
			{
				while (i > 0) /* Handle memory allocation failure */
					free(commands[--i]);
			free(commands);
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
 * parse_input - Parses input string into an array of arguments.
 * @input: input string from user.
 *
 * Description: Tokenizes the input string using spaces, tabs, and newlines
 * as delimiters, and stores each token in the args array.
 *
 * Return: A pointer to an array of strings (char **), where each string
 * is an argument parsed from the input.
 */
char **parse_input(char *input)
{
	char **args, *token;
	size_t bufsize = INIT_BUF_SIZE, position = 0;

	args = malloc(bufsize * sizeof(char *)); /* Allocate memory for args array */
	if (!args)
	{
		return (NULL);
	}
	token = custom_strtok(input, " \t"); /* Tokenize based on spaces and tabs*/
	while (token != NULL)
	{
		if (position >= bufsize)
		{
			bufsize *= 2;
			args = realloc(args, bufsize * sizeof(char *));

			if (!args)
				return (NULL);
		}
		args[position] = custom_strdup(token);
		if (!args)
			return (NULL);
		position++;
		token = custom_strtok(NULL, " \t");
	}
	args[position] = NULL;  /* Null-terminate the array */
	return (args);
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
	char *command_path = search_path(args[0]);

	if (command_path != NULL)
	{
		args[0] = command_path;
		execute_command(args, environ, program_name, line_number);
		free(command_path);
	}
	else
	{
		fprintf(stderr, "%s: %d: %s: not found\n", program_name,
				line_number, args[0]);
	}
}
