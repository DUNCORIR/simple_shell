#include "shell.h"

/**
 * parse_input - Parses input string into an array of arguments.
 * @input: input string from user.
 *
 * Description: tokenizes input string using spaces as delimiters
 * and store each token in args array.
 *
 * Return: A pointer to an array of strings (char **), where each string
 * is an argument parsed from the input.
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
