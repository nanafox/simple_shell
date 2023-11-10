#include "shell.h"

/**
 * execute_command - executes the command given
 * @pathname: path to the command
 * @argv: command line arguments
 *
 * Return: 0 on success, -1 on failure
 */
int execute_command(char *pathname, char *argv[])
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
		if (execve(pathname, argv, environ) == -1)
		{
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
