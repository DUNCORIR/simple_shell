#include "shell.h"

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

	(void)argc;
	(void)argv;

	while (1)
	{
		printf("$ "); /* Display prompt */
		fflush(stdout);

		nread = custom_getline(&input, &len);
		if (nread == -1) /* Read input from user. */
		{
			free(input);
			exit(EXIT_FAILURE); /* Exit normally on EOF */
		}
		/* Remove newline character if present */
		if (nread > 0 && input[nread - 1] == '\n')
		{
			input[nread - 1] = '\0';
		}
		/* Parse the input into arguments */
		args = parse_input(input);
		/* Execute command or handle built-in */
		execute_command_or_builtin(args, environ);
		free(args);
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
void handle_input(char **input, size_t *len, ssize_t *nread)
{
	char *equal_sign;
	char **args;

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
	equal_sign = strchr(*input, '='); /* Handle environment variable assignment */
	if (equal_sign != NULL)
	{
		char *var = strtok(*input, "="), *val = strtok(NULL, "=");

		if (var && val)
		{
			if (setenv(var, val, 1) == -1)
				perror("setenv"); /* Handle setenv error */
		}
		else
			fprintf(stderr, "Invalid environment variable assignment\n");
		return; /* Skip command execution */
	}
	args = parse_input(*input);
	if (args[0] != NULL)
	{
		handle_builtins(args, environ); /* Handle built-ins */
		execute_command(args, environ); /* Execute command */
	}
	free(args); /* Free argument list */
}

/**
 * execute_command_or_builtin - Executes a command or a built-in function
 * @args: Array of arguments parsed from input.
 * @environ: The environment variables.
 *
 * Description: Reads the user's input using getline, checks for errors,
 *              and handles the EOF condition.
 */
void execute_command_or_builtin(char **args, char **environ)
{
	char *command_path;

	if (args[0] != NULL)
	{
		if (strcmp(args[0], "exit") == 0)
		{
			execute_exit(args);
		}
		else if (strcmp(args[0], "env") == 0)
		{
			print_env(environ); /* handle built in function */
		}
		else if (strcmp(args[0], "setenv") == 0)
		{
			execute_setenv(args); /* Handle setenv built-in function */
		}
		else if (strcmp(args[0], "unsetenv") == 0)
		{
			execute_unsetenv(args); /*Handle unsetenv built-in function */
		}
		else
		{
			command_path = search_path(args[0]);
			if (command_path != NULL)
			{
				args[0] = command_path;
				execute_command(args, environ);
				free(command_path);
			}
			else
			{
				/* Print error if command not found */
				fprintf(stderr, "Command not found: %s\n", args[0]);
			}
		}
	}
}
