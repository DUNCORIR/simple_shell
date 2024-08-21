#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * execute_command - Executes a command if executable.
 * @input: command to be executed.
 */
void execute_command(char *input)
{
	pid_t pid;
	char *argv[2]; /* arg vector */
	char *const envp[] = { NULL }; /*Environment vector.*/

	pid = fork();

	if (pid == 0) /* childs process */
	{
		argv[0] = (char *)input;
		argv[1] = NULL;


		execve(input, argv, envp);
		/*ef it fails print error message */
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0) /*parent process */
	{
		wait(NULL);
	}
	else
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

/**
 * main - Entry point of simple shell program.
 *
 * The function runs a basic shell thar prompts for and executes user
 * commands.Exits on end-of-file and no advanced features.
 *
 * Return: Always 0 0n successful completion.
 */
int main(void)
{
	char *input = NULL;
	size_t len = 0;

	ssize_t nread;

	while (1)
	{
		printf("$ ");
		nread = getline(&input, &len, stdin);
		if (nread == -1)
		{
			if (feof(stdin))
			{
				free(input);
				exit(EXIT_SUCCESS);
			}
			perror("getline");
			free(input);
			exit(EXIT_FAILURE);
		}

		/*Remove newline character from input */
		input[strcspn(input, "\n")] = '\0';
		if (strlen(input) == 0)
			continue;
		if (access(input, X_OK) == 0)
		{
			execute_command(input);
		}
		else
		{
			fprintf(stderr, "%s%s\n", "Command not found: ", input);
		}
	}

	free(input);
	return (0);
}
