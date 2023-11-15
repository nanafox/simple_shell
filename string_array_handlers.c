#include "shell.h"

/**
 * duplicate_str_array - duplicate a string array
 * @original: the original string
 *
 * Return: the duplicate string on success, else NULL if it fails
 */
char **duplicate_str_array(char **original)
{
	size_t size, i;
	char **duplicate;

	if (original == NULL)
		return (NULL);

	size = 0;
	while (original[size] != NULL)
		size++;

	duplicate = (char **)malloc((size + 1) * sizeof(char *));
	if (duplicate == NULL)
		return (NULL);

	for (i = 0; i < size; i++)
	{
		duplicate[i] = _strdup(original[i]);
		if (duplicate[i] == NULL)
		{
			free_str(duplicate);
			return (NULL);
		}
	}
	duplicate[size] = NULL;

	return (duplicate);
}

/**
 * concatenate_arrays - appends the contents of one string array to another
 * @dest: a pointer to the destination string array
 * @src: the source string array (where to copy from)
 */
void concatenate_arrays(char ***dest, char **src)
{
	int i, src_length = 0, dest_length = 0;

	if (src == NULL)
		return; /* there's nothing concatenate */

	while ((*dest)[dest_length] != NULL)
		dest_length++;

	while (src[src_length] != NULL)
		src_length++;

	*dest = realloc(*dest, (dest_length + src_length + 1) * sizeof(char *));
	if (*dest == NULL)
		return; /* memory allocation failed */

	for (i = 0; i < src_length; i++)
	{
		(*dest)[dest_length + i] = _strdup(src[i]);
	}

	/* null terminate the string array */
	(*dest)[dest_length + src_length] = NULL;
}
