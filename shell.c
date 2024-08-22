#include "shell.h"

/**
 * Entry point for simple shell
 * @argc: Argumnet count
 * @argv: Argument vector
 *
 * Return: Always success.
 */
int main(int argc, char **argv)
{
	char *input = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;
	char *command_path;

	while (1)
	{
		printf("$ "); /* Display prompt */
		nread = getline(&input, &len, stdin); /* Read input from user. */
		if (nread == -1)
		{
			if (feof(stdin))
			{
				free(input);
				exit(EXIT_FAILURE); /* Exit normally on EOF */
			}
			perror("getline");
			exit(EXIT_FAILURE); /* Exit with failure on error */
		}
		args = parse_input(input);
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
					fprintf(stderr, "Command not found: %s\n", args[0]);
				}
			}
		}
		free(args);
	}
	free(input); /* Free the input string memory */
	return (EXIT_SUCCESS);
}
