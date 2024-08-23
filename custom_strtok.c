#include "shell.h"

/**
 * custom_strtok - Custom implementation of strtok
 * @str: String to tokenize, or NULL to continue tokenizing the previous string.
 * @delim: The delimiters used to separate tokens
 *
 * Return: A pointer to the next token, or NULL if there are no more tokens
 *
 * Description: Function splits the string `str` into tokens separated by
 *		any of the characters in `delim`.Should be called initially
 *		with `str` pointing to the string to be tokenized, and then
 *		subsequently with `str` as NULL to continue tokenizing the same
 *		string.
 */
char *custom_strtok(char *str, const char *delim)
{
	static char *saved_str;
	char *token;
	int start = 0;
	int end;
	
	/* If new string is given,use it else continue with the previous one */
	if (str != NULL)
		saved_str = str;
	/* If there's no string left to tokenize, return NULL */
	if (saved_str == NULL)
		return (NULL);

	/* Skip leading delimiters */
	while (saved_str[start] != '\0' && strchr(delim, saved_str[start]) != NULL)
		start++;

	/* If no more tokens */
	if (saved_str[start] == '\0')
	{
		return (NULL);
	}

	/* move to end token */
	end = start;
	while (saved_str[end] != '\0' && strchr(delim, saved_str[end]) == NULL)
	end++;

	/* Null terminate the token */
	if (saved_str[end] != '\0')
	{
		saved_str[end] = '\0'; /* End of the token */
		saved_str = NULL; /*No more tokens left */
	}
	else
	{
		saved_str[end] = '\0'; /* Null terminate the token */
		saved_str = saved_str + end + 1; /*Move to the next token */
	}

	/*  Return the current token */
	token = saved_str + start;
	return (token);
}
