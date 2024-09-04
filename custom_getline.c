#include "shell.h"

/**
 * custom_getline - Reads an entire line from stdin.
 * @lineptr: Pointer to the buffer where the input is stored.
 * @n: Pointer to the size of the buffer.
 * Return: Number of characters read, or -1 on failure or EOF.
 */
ssize_t custom_getline(char **lineptr, size_t *n)
{
	char buffer[STATIC_BUF_SIZE];
	ssize_t buffer_pos = 0, buffer_size = 0;
	ssize_t num_read = 0;
	char *new_lineptr, c;

	if (!lineptr || !n) /* Validate input */
		return (-1);
	if (!*lineptr) /* Allocate memory if needed */
	{
		*lineptr = malloc(INIT_BUF_SIZE);
		if (!*lineptr)
			return (-1);
		*n = INIT_BUF_SIZE;
	}
	while (1)
	{
		if (buffer_pos >= buffer_size)  /* Refill buffer if empty */
		{
			buffer_size = read(STDIN_FILENO, buffer, STATIC_BUF_SIZE);
			buffer_pos = 0;
			if (buffer_size <= 0) /* EOF or error */
			{
				if (num_read > 0) /* Return what has been read if any */
				{
					(*lineptr)[num_read] = '\0';/* Null-terminate the string */
					return (num_read);
				}
				free(*lineptr); /* Free mem before returning on error*/
				*lineptr = NULL;
				return (-1); /* No data read or error occurred */
			}
		}
		c = buffer[buffer_pos++]; /* Get next character */
		if (num_read >= (ssize_t)(*n - 1)) /* Resize if buffer full */
		{
			*n *= 2;
			new_lineptr = realloc(*lineptr, *n);
			if (!new_lineptr)
			{
				free(*lineptr); /* Free previously allocated memory */
				*lineptr = NULL;
				return (-1);
			}
			*lineptr = new_lineptr;
		}
		(*lineptr)[num_read++] = c; /* Store character */
		if (c == '\n') /* Stop at newline */
			break;
	}
	(*lineptr)[num_read] = '\0'; /* Null-terminate the string */
	return (num_read); /* Return chars read */
}
