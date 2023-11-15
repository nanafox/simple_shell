#include "shell.h"

/**
 * handle_with_path - handles commands when the PATH is set
 * @path_list: a list of pathnames in the PATH variable
 * @sub_command: the command to execute
 *
 * Return: the exit code of the child process, else -1 if the command is not in
 * the PATH provided
 */
int handle_with_path(path_t *path_list, char **sub_command)
{
	char path[BUFF_SIZE];

	while (path_list != NULL)
	{
		sprintf(path, "%s%s%s", path_list->pathname, "/", sub_command[0]);
		if (access(path, X_OK) == 0)
		{
			return (execute_command(path, sub_command));
		}
		else if (access(sub_command[0], X_OK) == 0)
		{
			return (execute_command(sub_command[0], sub_command));
		}
		path_list = path_list->next;
	}

	return (-1);
}

/**
 * handle_file_as_input - handles execution when a file is given as input on
 * the command line (non-interactive mode)
 * @filename: the name of file containing the commands
 * @path_list: a list of pathnames in the PATH variable
 *
 * Return: 0, or the exit status of the just exited process
 */
int handle_file_as_input(char *filename, path_t *path_list)
{
	char *line = NULL;
	size_t n = 0;
	int n_read, fd, exit_code = 0;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		/* we couldn't open the file, let's clean and leave */
		free_list(&path_list);
		dprintf(2, "%s: 0: Can't open %s\n", _getenv("msh"), filename);
		return (CMD_NOT_FOUND);
	}

	n_read = _getline(&line, &n, fd);
	close(fd);

	if (n_read == -1)
	{
		multi_free("sp", line, &path_list);
		return (-1); /* reading file failed */
	}

	if (n_read)
		exit_code = parse_line(line, path_list);

	multi_free("sp", line, &path_list);

	return (exit_code);
}
