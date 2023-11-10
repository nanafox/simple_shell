#include "shell.h"

static int exit_code; /* keeps track of all exit codes */

/**
 * parse_line - parses the receive command line, processes it before handing it
 * over to the executor only after it has confirmed the command is valid
 * @line: the command line received
 * @path_list: a list of pathnames in the PATH variable
 *
 * Return: the exit code of the executed program, else -1 if something goes
 * wrong
 */
int parse_line(char *line, path_t *path_list)
{
	char **commands = NULL;

	/* skip normal ENTER keys and leading comments */
	if (*line == '\n' || *line == '#')
		return (0);

	/* first of all, let's get rid of all comments */
	line = handle_comments(line);

	/* now let's all the commands provided by the user */
	commands = _strtok(line, "\n;");
	if (commands == NULL)
	{
		perror("_strtok");
		return (-1); /* an error occurred while getting the commands */
	}

	return (parse_and_execute(commands, path_list, line));
}

/**
 * parse_and_execute - parses each sub command line and executes it
 * @commands: an array of command line strings
 * @path_list: a list of pathnames in the PATH variable
 * @line: the command line received
 *
 * Return: the exit code of the executed program, else -1 if something goes
 * wrong
 */
int parse_and_execute(char **commands, path_t *path_list, char *line)
{
	size_t i;
	char **sub_command = NULL;

	for (i = 0; commands[i] != NULL; i++)
	{
		/* get the sub commands and work on them */
		sub_command = _strtok(commands[i], NULL);
		if (sub_command == NULL)
		{
			free_str(commands);
			return (0); /* probably just lots of tabs or spaces, maybe both */
		}
		sub_command = handle_variables(sub_command, exit_code);
		exit_code = handle_builtin(sub_command, commands, path_list,
				line, exit_code);
		if (exit_code != NOT_BUILTIN)
		{
			safe_free(commands[i]);
			free_str(sub_command);
			continue; /* shell builtin executed well */
		}
		if (path_list != NULL) /* handle the command with the PATH variable */
		{
			exit_code = handle_with_path(path_list, sub_command);
			if (exit_code == -1)
				exit_code = print_cmd_not_found(sub_command, commands, i);
		}
		else
		{
			if (access(sub_command[0], X_OK) == 0 && _strchr(sub_command[0], '/'))
				exit_code = execute_command(sub_command[0], sub_command);
			else
				exit_code = print_cmd_not_found(sub_command, commands, i);
		}
		safe_free(commands[i]);
		free_str(sub_command);
	}

	free_str(commands);
	return (exit_code);
}

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
 * print_cmd_not_found - prints the command not found error
 * @sub_command: the actual command executed
 * @commands: a list of commands received on the command line
 * @index: current index in the commands list
 *
 * Return: 127 command not found code, else 0
 */
int print_cmd_not_found(char **sub_command, char **commands, size_t index)
{
	static size_t err_count = 1;

	dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n",
		   _getenv("_"), err_count, sub_command[0]);
	err_count++;

	if (commands[index + 1] == NULL)
	{
		return (CMD_NOT_FOUND); /* command not found */
	}

	return (0);
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
	int n_read, fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		dprintf(2, "%s: 0: Can't open %s\n", _getenv("_"), filename);
		return (CMD_NOT_FOUND);
	}

	n_read = _getline(&line, &n, fd);
	close(fd);

	if (n_read == -1)
	{
		return (-1); /* reading file failed */
	}

	if (n_read)
		exit_code = parse_line(line, path_list);

	safe_free(line);

	return (exit_code);
}
