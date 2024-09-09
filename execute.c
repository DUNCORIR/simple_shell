#include "shell.h"

/**
 * handle_builtins - Handles built-in commands like "exit" and "env".
 * @args: Array of arguments.
 * @envp: The environment vector.
 * @program_name: The name of the shell program (for error messages).
 * @line_number: The line number of the command
 *		in the input (for error messages).
 *
 *
 * Return: 1 if handled, 0 otherwise.
 */
int handle_builtins(char **args, char **envp,
		char *program_name, int line_number)
{
	int last_status = 0;
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
		execute_exit(args, last_status);
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
 * @last_status: The status code of the last executed command.
 * @args: Array of arguments.
 * @envp: The environment vector.
 *
 * Return: Nothing.
 */
int execute_fork(char *cmd_path, char **args, char **envp, int *last_status)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0) /* Fork failed */
	{
		perror("Fork error");
		return (-1); /* Return error code */
	}
	else if (pid == 0) /* child process */
	{
		if (execve(cmd_path, args, envp) == -1)
		{
			perror(cmd_path); /* Print command name instead of 'execve' */
			_exit(127); /* Exit with status 2 to indicate failure (adjust as needed) */
		}
	}
	else /* Parent process */
	{
		do {
			/* Wait for the child process to change state */
			if (waitpid(pid, &status, 0) == -1)
			{
				perror("waitpid error");
				return (1); /* Return error code */
			}

		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		if (WIFEXITED(status))
		{
			*last_status = WEXITSTATUS(status); /* Return exit status child process */
		}
	}
	return (*last_status); /* Return 1 if there was an error */
}

/**
 * execute_command - Executes a command.
 * @args: array of arguments.
 * @envp: The environment vector.
 * @program_name: The name of the
 *	shell program (for error messages).
 * @line_number: The line number of the command in
 *	the input (for error messages).
 * @last_status: The status code of the last executed command.
 *
 * The function checks if the command is accessible and executable.
 * forks a new process to execute the command using execve if
 * executable.errpr message if no command found.
 *
 * Return: 0 on success, -1 on error.
 */
int execute_command(char **args, char **envp, char *program_name,
		int line_number, int last_status)
{
	char *cmd_path;
	int status;

	if (args == NULL || args[0] == NULL)
	{
		return (1); /* Handle empty or invalid args */
	}
	if (strcmp(args[0], "exit") == 0) /* Check for exit command */
	{
		execute_exit(args, last_status);
		return (1); /* Exit the shell */
	}

	if (strcmp(args[0], "cd") == 0) /*Handle `cd` command,especially `cd -` */
	{
		return (execute_cd(args));  /* Handle `cd - */
	}
	/* Check if the command is a built-in */
	if (handle_builtins(args, envp, program_name, line_number))
	{
		return (1);
	}
	cmd_path = search_path(args[0]); /* Search for the command in path */
	if (cmd_path == NULL)
	{
		/* Format error message like /bin/sh: ./hsh: 1: command: not found */
		fprintf(stderr, "%s: %d: %s: not found\n", program_name,
				line_number, args[0]);
		return (127);
	}
	status = execute_fork(cmd_path, args, envp, &last_status);
	/* Free the allocated memory for the command path */
	free(cmd_path);
	return (status);
}

/**
 * execute_command_or_builtin - Executes a command or a shell built-in.
 * @args: An array of arguments passed to the command.
 * @environ: An array of environment variables.
 * @program_name: The name of the shell program (argv[0]).
 * @line_number: The line number in the script or input being executed.
 *
 * Return: An integer status code, 1 if a built-in command was handled,
 * or 0 otherwise.
 */
int execute_command_or_builtin(char **args, char **environ,
		char *program_name, int line_number)
{
	char *command_path;

	if (args[0] == NULL)
		return (0);
	if (handle_builtin(args, environ, program_name, line_number))
		return (1);
	command_path = search_path(args[0]);

	if (command_path != NULL)
	{
		handle_external_command(args, environ, program_name, line_number);
		free(command_path); /* free the memory allocated by search_path */
		return (1);
	}
	else
	{
		fprintf(stderr, "%s: %d: %s: not found\n",
				program_name, line_number, args[0]);
		return (127);
	}
}
