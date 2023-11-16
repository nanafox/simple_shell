#include "shell.h"

static char *line;
static char **tokens;

/**
 * main - the entry point for the shell
 * @argc: command line arguments counter
 * @argv: command line arguments
 *
 * Return: 0 on success, or the exit code of the just exited process
 */
int main(int argc, char *argv[])
{
	size_t i, len = 0;
	ssize_t n_read = 0;
	int exit_code = 0;
	path_t *path_list = NULL;

	path_list = build_path(&path_list);
	if (argc >= 2)
	{
		exit_code = handle_file_as_input(argv, path_list);
		return (exit_code);
	}

	while (RUNNING)
	{
		show_prompt();
		fflush(stdout);

		n_read = _getline(&line, &len, STDIN_FILENO);
		if (n_read == 0) /* most definitely Ctrl+D, clean up and leave */
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			return (parse_line("exit", path_list, argv[0]));
		}

		tokens = _strtok(line, "\n");
		for (i = 0; tokens[i] != NULL; i++)
		{
			if ((!_strcmp(tokens[i], "exit") && tokens[i + 1] == NULL))
			{
				safe_free(tokens[i]);
				return (parse_line("exit", path_list, argv[0]));
			}
			exit_code = parse_line(tokens[i], path_list, argv[0]);
			safe_free(tokens[i]);
		}
		safe_free(tokens);
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

/**
 * clean - cleans up the memory allocated by the getline function on exit
 */
void clean(void)
{
	if (line != NULL)
		safe_free(line);

	if (tokens != NULL)
	{
		size_t i;

		for (i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
			tokens[i] = NULL;
		}
		free(tokens);
	}
}
