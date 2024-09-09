#include "shell.h"

/**
 * read_from_stdin - Reads data from stdin into a buffer.
 * @buffer: The buffer to store read data.
 * @buffer_size: The number of bytes read into the buffer.
 *
 * Return: The number of bytes read or -1 on error.
 *
 */
ssize_t read_from_stdin(char *buffer, ssize_t *buffer_size)
{
	(void)buffer;

	*buffer_size = read(STDIN_FILENO, buffer, STATIC_BUF_SIZE);
	if (*buffer_size <= 0) /* EOF or error */
		return (-1);
	return (0);
}

/**
 * resize_line_buffer - Resizes the line buffer if it is full.
 * @lineptr: Pointer to the line buffer.
 * @n: Pointer to the size of the buffer.
 *
 * Return: 1 on success, 0 on failure.
 */
int resize_line_buffer(char **lineptr, size_t *n)
{
	char *new_lineptr;

	*n *= 2;
	new_lineptr = realloc(*lineptr, *n);
	if (!new_lineptr) /* If realloc fails */
	{
		free(*lineptr);
		*lineptr = NULL;
		return (0);
	}
	*lineptr = new_lineptr;
	return (0);
}

/**
 * custom_getline - Reads an entire line from stdin.
 *
 * @lineptr: Pointer to the buffer where the input is stored.
 * @n: Pointer to the size of the buffer.
 * Return: Number of characters read, or -1 on failure or EOF.
 */
ssize_t custom_getline(char **lineptr, size_t *n)
{
	char buffer[STATIC_BUF_SIZE];
	ssize_t buffer_pos = 0, buffer_size = 0;
	ssize_t num_read = 0;
	char c;

	if (!lineptr || !n) /* Validate input */
		return (-1);
	if (!*lineptr) /* Allocate memory if needed */
	{
		*lineptr = malloc(INIT_BUF_SIZE);
		if (!*lineptr)
			return (-1);
		*n = INIT_BUF_SIZE;
	}
	while (1) /* Refill buffer if empty */
	{
		if (buffer_pos >= buffer_size)  /* Refill buffer if empty */
		{
			if (read_from_stdin(buffer, &buffer_size) == -1)
			return (handle_read_error(lineptr, &num_read));
			buffer_pos = 0;
		}
		c = buffer[buffer_pos++]; /* Get next character */
		if (num_read >= (ssize_t)(*n)) /* Resize if buffer full */
		{
			if (!resize_line_buffer(lineptr, n))
				return (-1);
		}
		(*lineptr)[num_read++] = c; /* Store character */
		if (c == '\n') /* Stop at newline */
			break;
	}
	(*lineptr)[num_read] = '\0'; /* Null-terminate the string */
	return (num_read); /* Return chars read */
}

/**
 * handle_read_error - Handles errors encountered during the reading process.
 *
 * @lineptr: Pointer to the buffer to free.
 * @num_read: Pointer to the number of bytes read.
 *
 * Return: Returns -1 to indicate an error.
 */
ssize_t handle_read_error(char **lineptr, ssize_t *num_read)
{
	/* Return what has been read if any */
	if (*num_read > 0)
	{
		(*lineptr)[*num_read] = '\0'; /* Null-terminate the string */
		return (*num_read);
	}
	/* Free memory before returning on error */
	free(*lineptr);
	*lineptr = NULL;
	return (-1);
}
