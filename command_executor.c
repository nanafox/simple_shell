#include "shell.h"

/**
 * execute_command - executes the command given
 * @pathname: the absolute path to the binary file to execute
 * @msh: the shell's context
 *
 * Return: 0 on success, -1 on failure
 */
int execute_command(const char *pathname, shell_t *msh)
{
	int status;
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid == 0)
	{
		if (execve(pathname, msh->sub_command, environ) == -1)
		{
			if (errno == EACCES)
			{
				fprintf(stderr, "%s: %lu: %s\n", msh->prog_name,
						++msh->cmd_count, strerror(errno));
				return (126);
			}
			perror("execve");
			return (-1);
		}
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("wait");
			return (-1);
		}
		if (WIFEXITED(status))
		{
			return (WEXITSTATUS(status));
		}
	}

	return (0);
}
