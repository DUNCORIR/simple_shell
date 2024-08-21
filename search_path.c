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
        char *path = strdup(path_env);/*Duplicate search path */
        char *cmd_path;
        char *token;
        struct stat st;

        /* checkmif command is an absolute path */
        if (command[0] == '/')
        {
                if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
                {
                        return (strdup(command)); /* Return absolute path */
                }
                else
                {
                        return (NULL); /* return if not executable */
                }
        }
        token = strtok(path, ":"); /* Duplicate PATH environment */
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
