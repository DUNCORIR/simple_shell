#include "shell.h"

/**
 * handle_builtins - Handles built-in commands like "exit" and "env".
 * @args: Array of arguments.
 * @envp: The environment vector.
 *
 * Return: 1 if handled, 0 otherwise.
 */
int handle_builtins(char **args, char **envp)
{
	if (args[0] == NULL)
		return (0);

	printf("Command: %s\n", args[0]);


	if (strcmp(args[0], "exit") == 0)
	{
		execute_exit(args);
		return (1);
	}
	if (strcmp(args[0], "env") == 0)
	{
		print_env(envp); /* call function to print environ variable */
		return (1);
	}
	if (strcmp(args[0], "setenv") == 0)
	{
		execute_setenv(args);
		return (1);
	}
	if (strcmp(args[0], "unsetenv") == 0)
	{
		execute_unsetenv(args);
		return (1);
	}

	return (0);
}

/**
 * execute_fork - Forks a process and executes the command.
 * @cmd_path: Full path to the command.
 * @args: Array of arguments.
 * @envp: The environment vector.
 *
 * Return: Nothing.
 */
static void execute_fork(char *cmd_path, char **args, char **envp)
{
	pid_t pid = fork();

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
}

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

	if (handle_builtins(args, envp))
		return;

	cmd_path = search_path(args[0]); /* Search for the command in path */
	if (!cmd_path)
	{
		fprintf(stderr, "Command not found: %s\n", args[0]);
		return;
	}
	printf("Executing: %s\n", cmd_path);
	execute_fork(cmd_path, args, envp);

	free(cmd_path);
}
