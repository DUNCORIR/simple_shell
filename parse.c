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

	token = custom_strdup(input);
	while (token != NULL && i < MAX_ARGS - 1)
	{
		args[i] = custom_strdup(token); /* Duplicate token into args array */
		if (args[i] == NULL)
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}

		i++;
		token = custom_strdup(NULL);

	}
	args[i] = NULL; /* Null-terminate array of arguments.*/

	return (args);
}

/**
 * strdup_custom - Custom implementation of strdup.
 * @str: The input string to duplicate.
 *
 * Return: A pointer to the duplicated string, or NULL on failure.
 */
char *custom_strdup(const char *str)
{
	char *dup_str;
	size_t len;
	size_t i;

	/* check NULL input */
	if (str == NULL)
		return (NULL);

	/* Get the length of input string */
	len = strlen(str);

	/* Allocate input string to the allocated memory */
	dup_str = malloc((len + 1) * sizeof(char));
	if (dup_str == NULL)
		return (NULL); /* Handle memory allocation failure */

	for (i = 0; i < len; i++)
	{
		dup_str[i] = str[i];
	}

	dup_str[len] = '\0';

	return (dup_str);
}
