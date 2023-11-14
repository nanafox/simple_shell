#include "shell.h"

/**
 * _strcmp - compare two strings
 * @s1: string 1
 * @s2: string 2
 *
 * Return: 0 if @s1 and @s2 are equal, a negative value if @s1 is
 * less than @s2, a positive value if @s1 is greater than @s2
 */
int _strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
		{
			return (0); /* Both strings are equal */
		}
		s1++;
		s2++;
	}

	return (*s1 - *s2);
}

/**
 * _strcat - concatenate two strings
 * @dest: destination string buffer
 * @src: source string buffer
 *
 * Description: The _strcat() function appends the @src string to the @dest
 * string, overwriting the terminating null byte ('\\0') at the end of dest,
 * and then adds a terminating null byte.
 *
 * Return: a pointer to the resulting string dest
 */
char *_strcat(char *dest, const char *src)
{
	int len, i;

	len = _strlen(dest);

	for (i = 0; src[i] != '\0'; i++)
	{
		dest[len++] = src[i];
	}
	dest[len] = '\0';

	return (dest);
}

/**
 * _strcpy - copy a string
 * @dest: destination buffer
 * @src: source buffer
 *
 * Description: copies the string pointed to by @src, including the terminating
 * null byte ('\\0'), to the buffer pointed to by @dest. The destination string
 * @dest must be large enough to receive the copy.
 *
 * Return: a pointer to the @dest string
 */
char *_strcpy(char *dest, const char *src)
{
	size_t i = 0;

	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';

	return (dest);
}

/**
 * _strlen - returns the length of a string
 * @s: string
 *
 * Return: length of string
 */
size_t _strlen(const char *s)
{
	int i = 0;

	if (s == NULL)
	{
		return (0);
	}

	while (s[i] != '\0')
	{
		i++;
	}

	return (i);
}

/**
 * get_operator - returns the && or || logical operator in a string if found
 * @str: the string to search
 *
 * Return: the logical operator if found, else NULL
 */
char *get_operator(char *str)
{
	char *operator = NULL;
	int i;

	if (str == NULL)
		return (NULL);

	/* Check for the "&&" operator */
	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '&' && str[i + 1] == '&')
		{
			operator = "&&";
			break;
		}
		/* Check for the "||" operator */
		else if (str[i] == '|' && str[i + 1] == '|')
		{
			operator = "||";
			break;
		}
	}

	return (operator);
}
