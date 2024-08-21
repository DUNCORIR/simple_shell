#include "shell.h"

/**
 * execute_command - Execute a command with arguments
 * @input: The command input by the user.
 *
 * function forks a new process to execute the command
 * with its arguments and waits for the process to complete.
 */
void execute_command(char **args, char **envp)
{
	pid_t pid;
	int i = 0;
	int status;
	char *token;
	char *input = NULL;
	char *argv[100];/* Array to hold command and arguments*/
	(void)envp; 
	(void)args;

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
