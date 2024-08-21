#include "shell.h"

/**
 * Entry point for simple shell.
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

        while (1)
        {
                printf("$ ");

                nread = getline(&input, &len, stdin);

                if (nread == -1)
                {
			if (feof(stdin))
			{
                        	free(input);
                        	exit(EXIT_FAILURE);
			}
			perror("getline");
			free(input);
			exit(EXIT_FAILURE);

                }
		/* REmove newline character */
                input[nread - 1] = '\0';

                args = parse_input(input);
               if (args[0] != NULL)
                {
                        execute_command(args, envp);
                }

		free(args);
        }

        free(input);
        return (0);
}
