#include "shell.h"

/**
 * _strtok - splits a string into words.
 * @str: The string to split
 * @delim: the delimeter to split on
 *
 * Return: a pointer to an array of strings (words) on success, NULL otherwise.
 */
char **_strtok(const char *str, const char *delim)
{
	char **str_array;
	int word_count, word_start, word_end; /* keeps track of words */
	int len, index, i;
	char *tmp_delim = (char *)delim;

	if (delim == NULL)
		tmp_delim = " \t\n"; /* assume spaces, tabs and newline */

	word_count = get_word_count(str, tmp_delim);
	if (word_count == 0)
		return (NULL); /* no valid words */

	str_array = malloc((word_count + 1) * sizeof(char *));
	if (str_array == NULL)
		return (NULL); /* memory allocation failed */

	word_start = index = 0;
	len = _strlen(str);

	for (i = 0; i < len; i++)
	{
		if ((!_strchr(tmp_delim, str[i]) && _strchr(tmp_delim, str[i + 1])) ||
			(!_strchr(tmp_delim, str[i]) && str[i + 1] == '\0'))
		{
			word_end = i + 1;
			str_array[index] = new_word(str, word_start, word_end);

			/* memory allocation for new word failed, clean up and leave */
			if (str_array[index] == NULL)
				return (free_str(str_array), NULL);
			index++;
		}
		else if (!_strchr(tmp_delim, str[i]) && !_strchr(tmp_delim, str[i + 1]))
			continue; /* still in a word, keep counting */
		else
			word_start = i + 1;
	}
	/* terminate the array */
	str_array[index] = NULL;
	return (str_array);
}

/**
 * _strpbrk - searches a string for any of a set of bytes
 * @s: string
 * @accept: substring
 *
 * Description: The _strpbrk() function locates the first occurrence in the
 * string @s of any of the bytes in the string @accept.
 *
 * Return: A pointer to the byte in @s that matches one of the bytes in
 * @accept, or NULL if no such byte is found.
 */
char *_strpbrk(const char *s, const char *accept)
{
	int i, j;

	for (i = 0; s[i] != '\0'; i++)
	{
		for (j = 0; accept[j]; j++)
		{
			if (s[i] == accept[j])
				return ((char *)&s[i]); /* match found*/
		}
	}
	return (NULL); /* no match found or end of string - return NULL */
}

/**
 * _strspn - get length of a prefix substring
 * @s: string
 * @accept: substring
 *
 * Description: The _strspn() function calculates the length (in bytes) of the
 * initial segment of @s which consists entirely of bytes in @accept.
 *
 * Return: The number of bytes in the initial segment of @s which
 * consist only of bytes from @accept.
 */
size_t _strspn(const char *s, const char *accept)
{
	size_t len = 0;
	int i, map[256] = {0};

	/* handle empty arguments */
	if (s == NULL || accept == NULL)
		return (0);

	/* use a hash map for efficient lookup - assume ASCII */
	for (i = 0; accept[i] != '\0'; i++)
		map[(unsigned char)accept[i]] = 1;

	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] == SPACE) /* search till next word */
			break;
		else if (map[(unsigned char)s[i]])
			len++; /* match found, increment length */
		else
			break; /* no match found */
	}
	return (len);
}

/**
 * _strrchr - locate character in string (reverse)
 * @s: the string
 * @c: the character to search
 *
 * Description: The _strrchr() function returns a pointer to the last
 * occurrence of the character @c in the string @s.
 *
 * Return: a pointer to the matched character or NULL if the character
 * is not found
 */
char *_strrchr(const char *s, int c)
{
	char *tmp_s = (char *)s;
	size_t len = _strlen(tmp_s);

	/* handle empty string */
	if (s == NULL)
		return (NULL);

	while (tmp_s[--len] != '\0')
	{
		if (tmp_s[len] == c)
			return (tmp_s + len); /* match found */
	}
	/* return a pointer to the null byte if 'c' is a null byte. */
	if (c == '\0')
		return (tmp_s);

	return (NULL); /* no match found or end of string - return NULL */
}

/**
 * get_word_count - Returns the number of words in a string.
 * @str: string
 * @delim: the delimeter to split on
 *
 * Return: Number of words
 */
int get_word_count(const char *str, const char *delim)
{
	int in_word = 0; /* flag to track if we are in a word or not */
	int word_count = 0;
	char *tmp_delim = (char *)delim;

	if (str == NULL || *str == '\0')
		return (0);

	if (delim == NULL)
		tmp_delim = " \t";

	while (*str)
	{
		if (_strchr(tmp_delim, *str))
		{
			in_word = 0; /* not in a word, reset flag */
		}
		else if (!in_word)
		{
			in_word = 1;  /* found the start of a new word */
			word_count++; /* count word */
		}
		str++;
	}

	return (word_count);
}
