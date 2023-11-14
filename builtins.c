#include "shell.h"

/**
 * _setenv - set a custom environment variable
 * @name: the name of the variable
 * @value: the value to set
 * @overwrite: if true, overwrite the variable if it exists
 *
 * Description: This function sets a custom environment variable with the given
 * name and value. If the variable already exists and overwrite is true, it
 * will be replaced.
 *
 * Return: 0 on success, 1 on error
 */
int _setenv(const char *name, const char *value, int overwrite)
{
	size_t len, i;
	char *env_var = NULL;

	if (name == NULL || name[0] == '\0' || _strchr(name, '=') != NULL)
	{
		fprintf(stderr, "Invalid variable name: %s\n", name);
		return (1);
	}
	/* check if the variable already exists */
	if (_getenv(name) != NULL)
	{
		if (overwrite)
		{
			if (_unsetenv(name) != 0)
				return (1);
		}
		else
			return (0); /* variable exists, and overwrite is false */
	}
	len = _strlen(name) + _strlen(value) + 2;
	env_var = malloc(len);
	if (env_var == NULL)
		return (1);

	sprintf(env_var, "%s=%s", name, value);
	for (i = 0; environ[i] != NULL; i++)
	{
		if (_strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
		{
			environ[i] = env_var;
			safe_free(env_var);
			return (0);
		}
	}
	/* the variable doesn't exist, create it */
	environ[i++] = env_var;
	environ[i] = NULL;
	safe_free(env_var);

	return (0);
}

/**
 * _unsetenv - unset a custom environment variable
 * @name: The name of the variable to unset
 *
 * Description: This function unsets a custom environment variable with the
 * given name.
 *
 * Return: 0 on success, 1 on error
 */
int _unsetenv(const char *name)
{
	size_t i, j, len;

	/* check for invalid variable names */
	if (name == NULL || name[0] == '\0' || _strchr(name, '=') != NULL)
	{
		fprintf(stderr, "Invalid variable name: %s\n", name);
		return (1);
	}

	len = _strlen(name);

	/* iterate through the environment variables and find the one to remove */
	for (i = 0; environ[i] != NULL; i++)
	{
		if (_strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
		{
			/* shift all environment variables after the one to be removed */
			for (j = i; environ[j] != NULL; j++)
			{
				environ[j] = environ[j + 1];
			}
			return (0);
		}
	}

	dprintf(STDERR_FILENO, "Variable not found: %s\n", name);
	return (1);
}

/**
 * handle_exit - handles the built-in `exit` command for the shell
 * @exit_code: the user provided exit code
 * @status: the exit status of the last executed process (used if the exit code
 * is not provided)
 * @cleanup: a pointer to the function that handles memory deallocation on exit
 * @commands: an array of command line strings
 * @sub_command: the command to execute
 * @path_list: a list of pathnames in the PATH variable
 * @aliases: a list of aliases
 * @line: the command line received
 *
 * Return: 2 on error, else exits with @exit_code
 */
int handle_exit(char *exit_code, int status,
		void (*cleanup)(const char *format, ...),
		char **sub_command, char **commands, char *line, path_t **path_list,
		alias_t **aliases)
{
	size_t illegal_num_count = 1;
	int code;

	if (exit_code == NULL)
	{
		cleanup("spatt", line, path_list, aliases, sub_command, commands);
		exit(status);
	}

	if (isalpha(*exit_code) || _atoi(exit_code) < 0 || *exit_code == '-')
	{
		dprintf(STDERR_FILENO, "%s: %lu: exit: Illegal number: %s\n",
				_getenv("_"), illegal_num_count, exit_code);
		illegal_num_count++;
		return (CMD_ERR);
	}

	code = _atoi(exit_code);
	cleanup("spatt", line, path_list, aliases, sub_command, commands);
	exit(code);
}

/**
 * handle_cd - handles the builtin `cd` command
 * @pathname: the string containing the path to change directory to
 *
 * Return: 0 on success, else 2 on error
 */
int handle_cd(const char *pathname)
{
	char *home = _getenv("HOME");
	char *oldpath = _getenv("OLDPWD");
	char pwd[BUFF_SIZE], *prog = _getenv("_");
	static size_t cd_err_count = 1;

	getcwd(pwd, BUFF_SIZE);
	oldpath = (oldpath) ? oldpath : pwd;
	if (pathname != NULL)
	{
		int dash = !_strcmp(pathname, "-") || !_strcmp(pathname, "--");
		char path[PATH_SIZE];

		/* build the full path when relative paths are given */
		if (!_strchr(pathname, '/') && !dash)
			sprintf(path, "%s/%s", pwd, ((dash) ? oldpath : pathname));
		else
			sprintf(path, "%s", ((dash) ? oldpath : pathname));
		if (chdir(path) == -1)
		{
			if (strspn(pathname, "-") > 2)
				dprintf(2, "%s: %lu: cd: Illegal option: --\n", prog, cd_err_count);
			else
				dprintf(2, "%s: %lu: cd: can't cd to %s\n", prog, cd_err_count, pathname);
			cd_err_count++;
			return (CMD_ERR);
		}
		if (dash)
			printf("%s\n", oldpath);
		setenv("OLDPWD", pwd, 1);
		getcwd(path, PATH_SIZE);
		setenv("PWD", path, 1);
	}
	else
	{
		if (chdir(home) == -1)
			return (CMD_ERR);
		setenv("OLDPWD", pwd, 1);
		setenv("PWD", home, 1);
	}
	return (0);
}

/**
 * _printenv - prints all environment variables
 */
void _printenv(void)
{
	int fd, status;
	char *filename = "/tmp/env";
	pid_t child;

	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return;
	if (write(fd, "env", 3) != -1)
	{
		char *argv[] = {"/bin/sh", "/tmp/env", NULL};

		child = fork();
		if (child == 0)
			execve(argv[0], argv, environ);
		else
			waitpid(child, &status, 0);
		close(fd);
	}
	else
	{
		close(fd);
		return;
	}
}
