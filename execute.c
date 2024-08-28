#include "shell.h"

/**
 * handle_builtins - Handles built-in commands like "exit" and "env".
 * @args: Array of arguments.
 * @envp: The environment vector.
 *
 * Return: 1 if handled, 0 otherwise.
 */
int handle_builtins(char **args, char **envp, char *program_name, int line_number)
{
	(void)program_name;  
	(void)line_number;

	if (args[0] == NULL)
		return (0);

	if (strcmp(args[0], "cd") == 0)
	{
		return (execute_cd(args));
	}
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
int execute_fork(char *cmd_path, char **args, char **envp)
{
	pid_t pid = fork();
	int status;

	pid = fork();
	if (pid == 0) /* Child process */
	{
		execve(cmd_path, args, envp);
		perror("execve");
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0) /* Parent process */
	{
		waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				return WEXITSTATUS(status);
	}
	else
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (1); /* Return 1 if there was an error */
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
int execute_command(char **args, char **envp, char *program_name, int line_number)
{
	char *cmd_path;
	int status;

	/* Check if the command is a built-in */
	if (handle_builtins(args, envp, program_name, line_number))
		return (0);

	cmd_path = search_path(args[0]); /* Search for the command in path */
	if (cmd_path == NULL)
	{
		/* Format error message like /bin/sh: ./hsh: 1: command: not found */
		fprintf(stderr, "%s: %d: %s: not found\n", program_name, line_number, args[0]);
		return (127);
	}

	/* Fork and execute the command */
	status = execute_fork(cmd_path, args, envp);
	/* Free the allocated memory for the command path */
	free(cmd_path);

	return (status);
}
