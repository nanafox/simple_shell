#include "shell.h"

/**
 * _getline - reads input from a file descriptor
 * @lineptr: a pointer to the string buffer
 * @n: number of bytes written
 * @fd: file descriptor
 *
 * Return: the number of bytes on success, else -1 on error
 */
ssize_t _getline(char **lineptr, size_t *n, int fd)
{
	ssize_t n_read;
	size_t total_read, buffer_size = BUFF_SIZE;

	if (*lineptr == NULL) /* check if we should allocate memory */
	{
		*lineptr = malloc(sizeof(char) * (buffer_size + 1));
		if (*lineptr == NULL)
			return (-1); /* memory allocation failed */
	}
	n_read = total_read = 0;
	while ((n_read = read(fd, *lineptr + total_read, BUFF_SIZE)) > 0)
	{
		total_read += n_read;
		/* check if the buffer needs to be resized */
		if (total_read >= buffer_size)
		{
			buffer_size *= 2;
			*lineptr = _realloc(*lineptr, total_read, buffer_size);
			if (*lineptr == NULL)
				return (-1); /* resizing buffer memory failed */
			*n = total_read;
		}
		if (total_read && (*lineptr)[total_read - 1] == '\n')
		{
			(*lineptr)[total_read] = '\0';
			*n = total_read;
			return (total_read);
		}
	}
	if (n_read == -1)
	{
		safe_free(*lineptr);
		return (-1); /* reading from file descriptor failed */
	}
	if (total_read == 0)
		safe_free(*lineptr); /* nothing was read, free the memory */

	return (total_read);
}
