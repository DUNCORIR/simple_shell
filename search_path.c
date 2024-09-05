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
	char *path, *cmd_path, *token;
	struct stat st;

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
	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(path);
			return (custom_strdup(command)); /* Return if command is executable */
		}
		free(path);
		return (NULL);
	}
	/* Tokenize the PATH and search each directory */
	token = custom_strtok(path, ":");
	while (token != NULL)
	{
		if (strlen(token) == 0)
		{
			token = ".";  /* Handle empty token as current directory (".") */
		}
		/* Allocate memory for full command path */

		cmd_path = malloc(strlen(token) + strlen(command) + 2);
		if (!cmd_path)
		{
			perror("malloc");
			free(path);
			return (NULL);
		}	
		/* Construct full path (directory + / + command) */
		sprintf(cmd_path, "%s/%s", token, command);
		/* Check if the command exists and is executable */
		if (stat(cmd_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(path); /* Free duplicated PATH */
			return (cmd_path); /* Return the found executable path */
		}	
		free(cmd_path); /* Free memory for command path */
		cmd_path = NULL;
		token = custom_strtok(NULL, ":"); /* Get next directory */
	}
	free(path); /* Free duplicated PATH after searching */
	return (NULL); /* Command not found in PATH */
}
	
