#include "shell.h"

/**
 * execute_command - executes command using execve
 * @args: array of arguments.
 * @envp: The environment vector.
 *
 * The function checks if the command is accessible and executable.
 * forks a new process to execute the command using execve if
 * executable.errpr message if no command found.
 */
void execute_command(char **args, char **envp)
{
	char *cmd_path;
	pid_t pid;

	cmd_path = search_path(args[0]);

	if (!cmd_path)
	{
		fprintf(stderr, "Command not found: %s\n", args[0]);
		return;
	}

	printf("Executing: %s\n", cmd_path);

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		execve(cmd_path, args, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(NULL);
	}

	free(cmd_path);
}
