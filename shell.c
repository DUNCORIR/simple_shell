#include "shell.h"

void handle_env_assignment(char *input);
void execute_commands_with_separator(char *input, char **argv, int line_number);

/**
 * main - Entry point for simple shell
 * @argc: Argumnet count
 * @argv: Argument vector.
 *
 * Description:Initializes the shell, displays the prompt, reads user input,
 *		and executes the corresponding command or built-in function
 *		in a loop until an exit condition is met..
 *
 * Return: Always success.
 */
int main(int argc, char **argv)
{
	char *input = NULL;
	size_t len = 0;
	ssize_t nread;
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
			if (feof(stdin))
			{
				free(input); /* Free memory on EOF */
				exit(EXIT_SUCCESS); /* Exit normally */
			}
			perror("getline"); /* Print error if getline fails */
			free(input); /* Free memory on error */
			exit(EXIT_FAILURE); /* Exit with failure on error */
		}
		/* Remove newline character if present */
		if (nread > 0 && input[nread - 1] == '\n')
		{
			input[nread - 1] = '\0';
		}
		/* Parse the input into arguments */
		args = parse_input(input);
		/* Execute command or handle built-in */
		if (args && args[0] != NULL) /* Ensure args are valid */
		{
			execute_command_or_builtin(args, environ, argv[0], line_number);
		}

		free(args); /* Free parsed arguments */
		line_number++; /* Increment line number for each input */
	}

	free(input); /* Free the input string memory */
	return (EXIT_SUCCESS);
}
/**
 * handle_input - Handles reading input from the user.
 * @input: Pointer to the input string.
 * @len: Pointer to the size of the buffer for getline.
 * @nread: Pointer to store the number of bytes read by getline.
 *
 * Description: This function reads the user's input using getline,
 * checks for errors, and handles the EOF condition.
 */
void handle_input(char **input, size_t *len, ssize_t *nread, char **argv, int line_number)
{
	char *equal_sign;

	*nread = custom_getline(input, len);/* Read input from user */
	if (*nread == -1) /* Check for errors or EOF */
	{
		if (feof(stdin))
		{
			free(*input); /* Free the allocated memory for input */
			exit(EXIT_SUCCESS); /* Exit normally on EOF */
		}
		perror("getline"); /* Print error message if getline fails */
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
	execute_commands_with_separator(*input, argv, line_number);
}

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

void execute_commands_with_separator(char *input, char **argv, int line_number)
{
	char **commands = parse_commands(input);

	int i;

	if (commands)
	{
		for (i = 0; commands[i]; i++)
		{
			char **args = parse_input(commands[i]);
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
 * execute_command_or_builtin - Executes a command or a built-in function
 * @args: Array of arguments parsed from input.
 * @environ: The environment variables.
 *
 * Description: Reads the user's input using getline, checks for errors,
 *              and handles the EOF condition.
 *
 * Return: 0 if the command was handled successfully or
 *	the command was executed.-1 if there was an error
 *	or the command was not found.
 */
int execute_command_or_builtin(char **args, char **environ, char *program_name, int line_number)
{
	char *command_path;

	if (args[0] != NULL)
	{
		if (strcmp(args[0], "exit") == 0)
		{
			execute_exit(args);
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
		command_path = search_path(args[0]);
		if (command_path != NULL)
		{
			args[0] = command_path;
			execute_command(args, environ, program_name, line_number);
			free(command_path);
		}
		else
		{
			/* Print error message with program name and line number */
			fprintf(stderr, "%s: %d: %s: not found\n", program_name, line_number, args[0]);
		}
	}
	return (0);
}
