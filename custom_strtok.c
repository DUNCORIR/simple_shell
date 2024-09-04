#include "shell.h"

/**
 * custom_strtok - Custom implementation of strtok
 * @str: String to tokenize, or NULL to continue
 *	 tokenizing the previous string.
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
	int start = 0, end = 0;

	if (str != NULL)
		saved_str = str;
	if (saved_str == NULL || *saved_str == '\0')
		return (NULL); /* Skip leading delimiters */
	while (saved_str[start] != '\0' && strchr(delim, saved_str[start]) != NULL)
		start++;
	if (saved_str[start] == '\0')  /* If no more tokens */
	{
		saved_str = NULL;
		return (NULL);
	}
	end = start; /* move to end token */
	while (saved_str[end] != '\0' && strchr(delim, saved_str[end]) == NULL)
	{
		if ((saved_str[end] == '&' && saved_str[end + 1] == '&') ||
				(saved_str[end] == '|' && saved_str[end + 1] == '|'))			{
			break;
		}
		end++;
	}
	 token = saved_str + start;
	if (saved_str[end] != '\0') /* Null terminate the token */
	{
		if ((saved_str[end] == '&' && saved_str[end + 1] == '&')
				|| (saved_str[end] == '|' && saved_str[end + 1] == '|'))
		{
			end++; /* Skip the second character of the special token */
		}
		saved_str[end] = '\0'; /* End of the token */
		saved_str = saved_str + end + 1; /*Move to the next token */
	}
	else
	{
		saved_str = NULL; /* No more tokens left */
	}
	return (token); /*  Return the current token */
}
