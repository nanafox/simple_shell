#include "shell.h"

/**
 * _itoa - convert integer to string
 *
 * @n: the integer to convert
 * @s: a char array to hold the conversion
 */
void _itoa(size_t n, char *s)
{
	int i = 0;

	do {
		s[i++] = n % 10 + '0'; /* get next digit */
	} while ((n /= 10) > 0);

	s[i] = '\0';

	_reverse(s, i);
}

/**
 * _reverse - reverse string in place
 *
 * @buffer: string to reverse
 * @len: length of the string
 */
void _reverse(char *buffer, size_t len)
{
	size_t i, mid;
	char c;

	if (!buffer)
		return; /* invalid string, nothing to do here */
	mid = len / 2;

	for (i = 0; i < mid; i++)
	{
		c = buffer[i];

		/* swap values */
		buffer[i] = buffer[len - i - 1];
		buffer[len - i - 1] = c;
	}
}


/**
 * _atoi - converts a string to an integer
 * @s: string
 *
 * Return: The converted value or 0 on error
 */
int _atoi(const char *s)
{
	unsigned int num = 0;
	int i, sign;

	sign = 1; /* assume positive sign */

	for (i = 0; !(isdigit(s[i])); i++)
	{
		if (s[i] == '\0') /* empty string or no digits */
			break;
		else if (isspace(s[i]) || isalpha(s[i]))
			continue; /* skip characters and whitespaces */
		else if (issign(s[i]))
			sign *= isnegative(s[i]); /* update sign */
	}

	while (isdigit(s[i]))
	{
		num = 10 * num + (s[i] - '0');
		i++;
	}

	return (num * sign);
}
