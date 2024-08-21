#include "main.h"

/**
 * main - Entry point for simple shell.
 *
 * Description: Prompts user for command then executes it.
 * Return: Always success.
 */
int main(void)
{
	char *input = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;
	char *envp[] = { NULL };
	size_t i;

	while (1)
	{
		printf("$ ");

		nread = getline(&input, &len, stdin);

		if (nread == -1)
		{
			perror("getline");
			free(input);
			exit(EXIT_FAILURE);
		}
		input[nread - 1] = '\0';

		args = parse_input(input);
		if (args[0] != NULL)
		{
			execute_command(args, envp);
		}
		/* Free allocated memory for arguments */
		for (i = 0; args[i] != NULL; i++)
		{
			free(args[i]);
		}
		
		free(args);
	}
	free(input);
	return (0);
}
/**
 * parse_input - Parses input string into an array of arguments.
 * @input: input string from user.
 * @args: array to store parsed arguments.
 *
 * Description: tokenizes input string using spaces as delimiters
 * and store each token in args array.
 */
char **parse_input(char *input)
{
	size_t i = 0;
	char **args;
	char *token;

	/* tokenize input string */
	args = malloc(MAX_ARGS * sizeof(char *));
	if (args == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(input, " ");
	while (token != NULL && i < MAX_ARGS - 1)
	{
		args[i] = strdup(token);
		if (args[i] == NULL)
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}
		i++;
		token = strtok(NULL, " ");

	}
	args[i] = NULL; /* Null-terminate array of arguments.*/
	
	return (args);
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

/**
 * search_path - Searches for a command in PATH environment variable.
 * @command: command to search for.
 *
 * The function iterates through directories in PATH variable,
 * checking if command is executable in any of them.
 *
 * Return: Full path to command if found, or NULL if not found.
 */
char *search_path(const char *command)
{
	char *path_env = getenv("PATH");
	char *path = strdup(path_env); /*Duplicate search path to avoid modifying origninal */
	char *cmd_path;
	char *token;
	struct stat st;
	
	/* checkmif command is an absolute path */

	if (command[0] == '/')
	{
		if(stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			return strdup(command); /* Return absolute path */
		}
		else

		{
			return (NULL); /* return if not executable */
		}
	}

	/* Duplicate PATH environment to avoid modifying it .*/
	token = strtok(path, ":");

	while (token)
	{
		/* Allocates memory for full command path */
		cmd_path = malloc(strlen(token) + strlen(command) + 2);
		if (!cmd_path)
		{
			perror("malloc");
			return (NULL);
		}
		/* Construct full command path */
		sprintf(cmd_path, "%s/%s", token, command);
		/* check if command is executable */
		if (stat(cmd_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			return (cmd_path);
		}

		/* free allocated mem and move to next directory */
		free(cmd_path);
		token = strtok(NULL, ":");
	}

	return (NULL);
}
