#include "shell.h"

/**
 * _strncmp - compare at most n bytes of two strings
 * @s1: the first string to be compared
 * @s2: the second string to be compared
 * @n: the maximum number of bytes to compare
 *
 * Return: 0 if the strings are equal, a negative value if s1 is less than s2,
 *         or a positive value if s1 is greater than s2
 */
int _strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i = 0;

	while (*s1 == *s2 && i < n)
	{
		if (*s1 == '\0' || i + 1 == n)
		{
			/* both strings are equal or reached the end of comparison */
			return (0);
		}
		s1++;
		s2++;
		i++;
	}

	return ((*s1) - (*s2));
}

/**
 * _strchr - locate character in string
 * @s: the string
 * @c: the character to search
 *
 * Description: The _strchr() function returns a pointer to the first
 * occurrence of the character @c in the string @s.
 *
 * Return: a pointer to the matched character or NULL if the character
 * is not found
 */
char *_strchr(const char *s, int c)
{
	char *tmp_s = (char *)s;

	/* handle empty string */
	if (s == NULL)
		return (NULL);

	while (*tmp_s != '\0')
	{
		if (*tmp_s == c)
			return (tmp_s); /* match found */
		tmp_s++;
	}

	/* return a pointer to the null byte if 'c' is a null byte. */
	if (c == '\0')
		return (tmp_s);

	return (NULL); /* no match found or end of string - return NULL */
}

/**
 * _strstr - locate a substring
 * @haystack: string
 * @needle: substring
 *
 * Description: The strstr() function finds the first occurrence of the
 * substring @needle in the string @haystack. The terminating null bytes('\\0')
 * are not compared.
 *
 * Return: return a pointer to the beginning of the located substring,
 * or NULL if the substring is not found.
 */
char *_strstr(const char *haystack, const char *needle)
{
	size_t i, j;

	for (i = 0; haystack[i]; i++)
	{
		j = 0;

		/* keep searching so long as characters in both strings are equal */
		while (haystack[i + j] == needle[j] && needle[j])
			j++;

		/* end of substring reached - it means there was a full match */
		if (needle[j] == '\0')
		{
			return ((char *)&haystack[i]);
		}
	}

	return (NULL); /* no match found */
}

/**
 * _strdup - Duplicates a string.
 * @str: string to duplicate
 *
 * Description: The _strdup() function returns a pointer to a newly allocated
 * space in memory, which contains a copy of the string @str. Memory for the
 * new string is obtained with malloc() and can be freed with free()
 *
 * Return: A pointer to the duplicated string, NULL if it fails
 */
char *_strdup(const char *str)
{
	char *dup_str;
	size_t i = 0;

	if (str == NULL)
		return (NULL); /* handle invalid string */

	dup_str = malloc(_strlen(str) + 1);

	if (dup_str == NULL)
		return (NULL);

	/* duplicate string - a copy process */
	while (str[i])
	{
		dup_str[i] = str[i];
		i++;
	}
	dup_str[i] = '\0';

	return (dup_str);
}

/**
 * _strncpy - copy a string
 * @dest: destination string buffer
 * @src: source string buffer
 * @n: number of bytes to write
 *
 * Description: Copy @n bytes from the string @s to @dest. If there is no null
 * byte among the first @n bytes of @s, the string in @dest will not be
 * null-terminated.
 *
 * Return: a pointer to the destination string @dest
 */
char *_strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}

	while (i < n)
	{
		/* ensure exactly n bytes are written */
		dest[i] = '\0';
		i++;
	}

	return (dest);
}
