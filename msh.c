#include "shell.h"

/**
 * main - the entry point for the shell
 * @argc: command line arguments counter
 * @argv: command line arguments
 *
 * Return: 0 on success, or the exit code of the just exited process
 */
int main(int argc, char *argv[])
{
	char *line = NULL;
	size_t len = 0;
	ssize_t n_read = 0;
	int exit_code = 0;
	path_t *path_list = NULL;

	setenv("msh", argv[0], 1); /* keep track of the program's name */
	path_list = build_path(&path_list);
	if (argc >= 2)
	{
		exit_code = handle_file_as_input(argv[1], path_list);
		return (exit_code);
	}
	while (RUNNING)
	{
		show_prompt();
		fflush(stdout);

		n_read = _getline(&line, &len, STDIN_FILENO);
		if (n_read == -1)
		{
			safe_free(line);
			perror("_getline");
			return (-1);
		}
		if (n_read == 0) /* most definitely Ctrl+D, clean up and leave */
		{
			safe_free(line);
			line = _strdup("exit");
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			return (parse_line(line, path_list));
		}
		exit_code = parse_line(line, path_list);
		safe_free(line);
	}

	return (exit_code);
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
