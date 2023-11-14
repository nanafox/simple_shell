#include "shell.h"

/**
 * handle_variables - handles variables passed to the shell
 * @command: the command to process for variables
 * @exit_code: the exit code of the last process
 *
 * Return: the updated command with variables expanded if one was found, else
 * the @command is returned as received with no modifications
 */
char **handle_variables(char **command, int exit_code)
{
	size_t i;
	ssize_t offset;
	char *value, *loc, result[32];

	for (i = 0; command[i] != NULL; i++)
	{
		loc = _strchr(command[i], '$');
		if (loc == NULL)
			/* no variables found yet? keep, searching till the end */
			continue;

		offset = (&loc[0]) - (&command[i][0]);

		if (command[i][offset + 1] == '\0')
			continue; /* this is just a literal '$' sign, ignore it */

		/* what's my PID? that's what this guy tells you */
		if (command[i][offset + 1] == '$')
		{
			_itoa(getpid(), result);
			safe_free(command[i]);
			command[i] = _strdup(result);
		}
		/* this is for when the user wants the exit code of the last process */
		else if (command[i][offset + 1] == '?')
		{
			_itoa(exit_code, result);
			safe_free(command[i]);
			command[i] = _strdup(result);
		}
		/* this is for when something like '$HOSTNAME' is received */
		else
		{
			value = _getenv(loc + 1);
			safe_free(command[i]);
			command[i] = _strdup(value);
		}
	}

	return (command);
}
