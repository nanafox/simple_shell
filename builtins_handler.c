#include "shell.h"

/**
 * handle_builtin - handles the builtin commands
 * @msh: contains all the data relevant to the shell's operation
 *
 * Return: exit code
 */
int handle_builtin(shell_t *msh)
{
	if (!_strcmp(msh->sub_command[0], "env") ||
		!_strcmp(msh->sub_command[0], "printenv"))
	{
		_printenv();
		return (0);
	}
	else if (!_strcmp(msh->sub_command[0], "exit"))
	{
		return (handle_exit(msh, multi_free));
	}

	/* let's handle the builtin 'cd' command */
	else if (!_strcmp(msh->sub_command[0], "cd"))
		return (handle_cd(msh));

	else if (!_strcmp(msh->sub_command[0], "setenv"))
	{
		if (msh->sub_command[1] && msh->sub_command[2])
			return (setenv(msh->sub_command[1], msh->sub_command[2], 1));
		return (1); /* invalid number of parameters received */
	}
	else if (!_strcmp(msh->sub_command[0], "unsetenv"))
		return (_unsetenv(msh->sub_command[1]));

	return (NOT_BUILTIN); /* not a builtin command */
}
