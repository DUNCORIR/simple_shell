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

	home_dir = getenv("HOME"); /* Get the HOME environment variable */
	new_dir = args[1]; /* Directory to change to */
	oldpwd = getenv("OLDPWD"); /* Get the OLDPWD environment variable */

	if (args[1] != NULL)
	{
		new_dir = args[1]; /* Directory to change to */
	}
	else
	{
		new_dir = home_dir ? home_dir : "."; /* Default to hom if none given */
	}

	if (new_dir[0] == '-' && new_dir[1] == '\0') /* 'cd -'to prev directory*/
	{
		if (!oldpwd)  /* Check if OLDPWD is set */
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
				return (-1);
		}
		new_dir = oldpwd; /* Set new_dir to OLDPWD */
		printf("%s\n", new_dir); /* Print the new directory for 'cd -' */
	}

	if (chdir(new_dir) != 0) /* Change directory and check for success */
	{
		perror("cd");
		return (-1);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL) /* OLDPWD with current directory */
	{
		setenv("OLDPWD", cwd, 1);
	}
	else
	{
		perror("getcwd");
		return (-1);
	}
	setenv("PWD", new_dir, 1); /* Update PWD with the new directory */
	return (0);
}
