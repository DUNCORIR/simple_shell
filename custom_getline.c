#include "shell.h"

/**
 * custom_getline - Reads an entire line from stdin.
 * @lineptr: Pointer to the buffer where the input is stored.
 * @n: Pointer to the size of the buffer.
 * Return: Number of characters read, or -1 on failure or EOF.
 */
ssize_t custom_getline(char **lineptr, size_t *n)
{
	static char buffer[STATIC_BUF_SIZE];
	static ssize_t buffer_pos = 0, buffer_size = 0;
	ssize_t num_read = 0;
	char *new_lineptr, c = 0;

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
                		return (buffer_size == 0 && num_read > 0) ? num_read : -1;
		}
		c = buffer[buffer_pos++]; /* Get next character */
		if (num_read >= (ssize_t)(*n - 1)) /* Resize if buffer full */
		{
			new_lineptr = realloc(*lineptr, *n * 2);
			if (!new_lineptr)
				return (-1);
			*lineptr = new_lineptr;
			*n *= 2;
		}
		(*lineptr)[num_read++] = c; /* Store character */
		if (c == '\n') /* Stop at newline */
			break;
	}
	(*lineptr)[num_read] = '\0'; /* Null-terminate the string */
	return (num_read); /* Return chars read */
}
