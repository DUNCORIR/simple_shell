#include "shell.h"

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
	char *path, *cmd_path;

	if (!path_env || strlen(path_env) == 0)
	{
		return (NULL);  /* Return NULL if PATH is empty or unset */
	}
	path = custom_strdup(path_env); /* Duplicate PATH */
	if (!path)
	{
		perror("custom_strdup");
		return (NULL);
	}
	/* Handle absolute or relative paths (those starting with / or .) */
	if (is_absolute_or_relative_path(command))
	{
		free(path);
		return (custom_strdup(command));
	}
	cmd_path = search_in_path(command, path); /* Search the command in PATH */
	free(path); /* Free duplicated PATH after searching */
	return (cmd_path);  /* Return the found executable path or NULL */
}
/**
 * is_absolute_or_relative_path - Checks if the command is
 * an absolute or relative path.
 * @command: The command to check.
 *
 * Return: 1 if the command is an absolute/relative path
 * and executable, 0 otherwise.
 */
int is_absolute_or_relative_path(const char *command)
{
	struct stat st;

	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			return (1); /*command is executable */
		}
		return (0);
	}
	return (0);
}

/**
 * search_in_path - Searches for the command in the
 * directories specified in PATH.
 * @command: The command to search for.
 * @path: The PATH string to search in.
 *
 * Return: The full path to the command if found
 * and executable, otherwise NULL.
 */
char *search_in_path(const char *command, char *path)
{
	char *token = custom_strtok(path, ":");
	char *cmd_path;
	struct stat st;

	while (token != NULL)
	{
		if (strlen(token) == 0)
		{
			token = ".";  /* Handle empty token as current directory (".") */
		}
		/* Allocate memory for full command path */

		cmd_path = construct_cmd_path(token, command); /* full command path */
		if (!cmd_path)
		{
			return (NULL);
		}
		/* Check if the command exists and is executable */
		if (stat(cmd_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			return (cmd_path); /* Return the found executable path */
		}
		free(cmd_path); /* Free memory for command path */
		token = custom_strtok(NULL, ":"); /* Get next directory */
	}
	return (NULL); /* Command not found in PATH */
}

/**
 * construct_cmd_path - Constructs the full path for
 * the command in the given directory.
 * @dir: The directory to append the command to.
 * @command: The command to append to the directory.
 *
 * Return: The full path of the command or NULL on memory allocation failure.
 */
char *construct_cmd_path(const char *dir, const char *command)
{
	char *cmd_path = malloc(strlen(dir) + strlen(command) + 2);

	if (!cmd_path)
	{
		perror("malloc");
		return (NULL);
	}
	sprintf(cmd_path, "%s/%s", dir, command);  /* Construct the full path */
	return (cmd_path);
}
