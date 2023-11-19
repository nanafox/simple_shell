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
	msh->err_count = 0;
	msh->exit_code = 0;

	return (msh);
}

/**
 * show_prompt - shows the prompt in interactive mode
 */
void show_prompt(void)
{
	char prompt[PROMPT_SIZE];
	char *username = _getenv("USER");
	char *pwd;

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

			sprintf(prompt, "[%s@msh %s]%% ", username,
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
