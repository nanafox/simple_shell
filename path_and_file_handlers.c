#include "shell.h"

/**
 * handle_with_path - handles commands when the PATH is set
 * @msh: contains all the data relevant to the shell's operation
 *
 * Return: the exit code of the child process, else -1 if the command is not in
 * the PATH provided
 */
int handle_with_path(shell_t *msh)
{
	char path[BUFF_SIZE];
	path_t *path_list = msh->path_list;

	while (path_list != NULL)
	{
		sprintf(path, "%s%s%s", path_list->pathname, "/", msh->sub_command[0]);
		if (access(path, X_OK) == 0)
		{
			return (execute_command(path, msh));
		}
		else if (access(msh->sub_command[0], X_OK) == 0)
		{
			return (execute_command(path, msh));
		}
		path_list = path_list->next;
	}

	return (-1);
}

/**
 * handle_file_as_input - handles execution when a file is given as input on
 * the command line (non-interactive mode)
 * @filename: the name of the file to read from
 * @msh: contains all the data relevant to the shell's operation
 *
 * Return: 0, or the exit status of the just exited process
 */
void handle_file_as_input(const char *filename, shell_t *msh)
{
	size_t n = 0;
	int n_read, fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		/* we couldn't open the file, let's clean and leave */
		free_list(&msh->path_list);
		fprintf(stderr, "%s: 0: Can't open %s\n", msh->prog_name, filename);
		exit(CMD_NOT_FOUND);
	}

	n_read = _getline(&msh->line, &n, fd);

	/*
	 * let us know if there was an error while closing file descriptor but
	 * continue any way
	 */
	if (close(fd) == -1)
		fprintf(stderr, "An error occurred while closing file descriptor #%d\n", fd);

	if (n_read == -1)
	{
		msh->exit_code = -1;
		handle_exit(msh, multi_free);
	}

	if (n_read)
	{
		msh->prog_name = filename;
		parse_line(msh);
	}

	handle_exit(msh, multi_free);
}
