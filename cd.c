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

	home_dir = getenv("HOME"); /* Get the HOME environment variable */
	new_dir = args[1]; /* Directory to change to */
	oldpwd = getenv("OLDPWD"); /* Get the OLDPWD environment variable */

	if (!new_dir)
		new_dir = home_dir ? home_dir : "."; /* Default to hom if none given */
	if (strcmp(new_dir, "-") == 0) /* Handle 'cd -'to previous directory */
	{
		if (!oldpwd)  /* Check if OLDPWD is set */
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
		}
		new_dir = oldpwd; /* Set new_dir to OLDPWD */
	}

	if (chdir(new_dir) != 0) /* Change directory and check for success */
	{
		perror("cd");
		return (-1);
	}

	setenv("OLDPWD", getenv("PWD"), 1); /* Update OLDPWD with current PWD */
	setenv("PWD", new_dir, 1); /* Update PWD with new directory */
	return (0);
}
