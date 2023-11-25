#include "shell.h"

/**
 * init_shell - initializes shell data
 *
 * Return: shell data initialized
 */
shell_t *init_shell(void)
{
	shell_t *msh = malloc(sizeof(shell_t));

	if (msh == NULL)
	{
		fprintf(stderr, "Launching shell failed.\n"
				"Please ensure you have enough system resources for this operation\n");
		exit(-1);
	}

	msh->path_list = NULL;
	msh->aliases = NULL;
	msh->line = NULL;
	msh->sub_command = NULL;
	msh->commands = NULL;
	msh->tokens = NULL;
	msh->token = NULL;
	msh->cmd_count = 0;
	msh->exit_code = 0;

	return (msh);
}

/**
 * get_hostname - returns the hostname of the system using the '/ect/hostname'
 * file
 * @buffer: the buffer to write the hostname to, it should large enough.
 *
 * Return: the hostname if found, else defaults to using 'msh' when anything
 * goes wrong
 */
char *get_hostname(char *buffer)
{
	int fd, n_read;

	fd = open("/etc/hostname", O_RDONLY);

	/* let's check whether the file opening failed */
	if (fd == -1)
	{
		/* looks like it did, fall back to using 'msh' as the hostname */
		_strcpy(buffer, "msh");
		return (buffer);
	}

	/* file opening was successful, let's grab the hostname */
	n_read = read(fd, buffer, 100);

	/* one more time, let's check for read failures and fall back as needed */
	if (n_read == -1 || n_read == 0)
		_strcpy(buffer, "msh");
	else
		buffer[n_read - 1] = '\0'; /* hostname was succesfully grabbed, use it */

	close(fd);

	return (buffer);
}
/**
 * show_prompt - shows the prompt in interactive mode
 */
void show_prompt(void)
{
	char prompt[PROMPT_SIZE], hostname[100];
	char *username = _getenv("USER"), *pwd;

	if (username != NULL)
	{
		pwd = _getenv("PWD");
		if (pwd != NULL)
		{
			/* get the right directory name to show on the prompt */
			pwd = (*pwd == '/' && *(pwd + 1) == '\0')
					  ? pwd
					  : (_strrchr(pwd, '/') +
						 1); /* show only the current directory */

			sprintf(prompt, "[%s@%s %s]%% ", username, get_hostname(hostname),
					(!_strcmp(pwd, username))
						? "~" /* show '~' for the user's $HOME directory */
						: pwd);
		}
	}
	else
	{
		/*
		 * there was not enough environment variables to build a much more
		 * customized prompt, fall back to the minimal prompt
		 */
		sprintf(prompt, "msh%% ");
	}

	/* show the prompt in interactive modes only */
	if (isatty(STDIN_FILENO))
		printf("%s", prompt);
}

/**
 * sigint_handler - handles signal interrupts (Ctrl+C)
 * @signum: signal number (unused)
 */
void sigint_handler(__attribute__((unused))int signum)
{
	putchar('\n');
	show_prompt();
	fflush(stdout);
}
