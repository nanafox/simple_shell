#include "shell.h"

/**
 * handle_builtin - handles the builtin commands
 * @commands: an array of command line strings
 * @sub_command: the actual command
 * @path_list: a list of pathnames in the PATH variable
 * @line: the command line received
 * @exit_code: the exit code to use
 *
 * Return: exit code
 */
int handle_builtin(char **sub_command, char **commands, path_t *path_list,
		char *line, int exit_code)
{
	if (!_strcmp(sub_command[0], "env") || !_strcmp(sub_command[0], "printenv"))
	{
		_printenv();
		return (0);
	}
	else if (!_strcmp(sub_command[0], "exit"))
	{
		return (handle_exit(sub_command[1], exit_code, multi_free,
					sub_command, commands, &path_list, line));
	}

	/* let's handle the builtin 'cd' command */
	else if (!_strcmp(sub_command[0], "cd"))
		return (handle_cd(sub_command[1]));

	else if (!_strcmp(sub_command[0], "setenv"))
	{
		if (sub_command[1] && sub_command[2])
			return (setenv(sub_command[1], sub_command[2], 1));
		return (1); /* inavlid number of parameters received */
	}
	else if (!_strcmp(sub_command[0], "unsetenv"))
		return (_unsetenv(sub_command[1]));

	return (NOT_BUILTIN); /* not a builtin command */
}
