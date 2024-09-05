#include "shell.h"

/**
 * execute_cd - Implements the cd command to change directories.
 * @args: Array of arguments (args[0] is "cd",
 *	args[1] is the directory).
 *
 * Return: 0 on success, -1 on failure.
 */
int execute_cd(char **args)
{
	char *home_dir, *oldpwd, *new_dir;
	char cwd[1024]; /* Buffer to store the current working directory */
	char *current_dir;

	home_dir = getenv("HOME"); /* Get the HOME environment variable */
	oldpwd = getenv("OLDPWD"); /* Get the OLDPWD environment variable */

	if (args[1] && args[1][0] == '-' && args[1][1] == '\0') /* 'cd -'*/
	{
		if (!oldpwd)  /* Check if OLDPWD is set */
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
				return (-1);
		}
		new_dir = oldpwd; /* Set new_dir to OLDPWD */
		printf("%s\n", new_dir);
	}
	else
	{
		new_dir = args[1] ? args[1] : home_dir ? home_dir : ".";
	}
	current_dir = getcwd(cwd, sizeof(cwd)); /* Get the current directory */
	if (chdir(new_dir) != 0) /* Change directory and check for success */
	{
		fprintf(stderr, "./hsh: 1: cd: can't cd to %s\n", new_dir);
		return (-1);
	}
	if (current_dir != NULL) /* OLDPWD with current directory */
	{
		setenv("OLDPWD", current_dir, 1);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		setenv("PWD", cwd, 1);
	}
	else
	{
		perror("getcwd");
		return (-1);
	}
	return (0);
}
