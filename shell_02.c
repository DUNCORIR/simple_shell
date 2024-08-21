#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * execute_command - Execute a command with arguments
 * @input: The command input by the user.
 *
 * function forks a new process to execute the command
 * with its arguments and waits for the process to complete.
 */
void execute_command(char *input)
{
	pid_t pid;
	int status;

	char *argv[100];/* Array to hold command and arguments*/
	char *token;
	int i = 0;

	/* tokenize input into command and arguments */
	token = strtok(input, " ");
	while (token != NULL)
	{
		argv[i++] = token;
		token = strtok(NULL, "");
	}
	argv[i] = NULL; /* NULL-terminating argument vector */

	if (access(argv[0], X_OK) != 0)
	{
		fprintf(stderr, "Command not found: %s\n", argv[0]);
		return;
	}
	pid = fork();

	if (pid == -1)/*childs process */
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execve(argv[0], argv, NULL) == -1)/* Execute command */
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else /* parent process*/
	{
		waitpid(pid, &status, 0);/*Child process to terminate */
	}
}

/**
 * main - Entry point of the shell program.
 *
 * function displays a prompt, reads user input, and
 * executes the command. Handles end-of-file and errors.
 *
 * Return: Always 0 on successful completion
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

		input[strcspn(input, "\n")] = '\0';

		if (strlen(input) == 0)
		{
			continue;
		}

		execute_command(input);
	}

	free(input);
	return (0);
}
