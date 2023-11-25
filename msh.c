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
	size_t len = 0;
	ssize_t n_read = 0;
	shell_t *msh = NULL;

	/* scout for singal interrupts (Ctrl + C) */
	signal(SIGINT, sigint_handler);

	msh = init_shell();
	msh->prog_name = argv[0];
	build_path(&msh->path_list);

	if (argc >= 2)
	{
		handle_file_as_input(argv[1], msh);
	}

	while (RUNNING)
	{
		show_prompt();
		fflush(stdout);

		n_read = _getline(&msh->line, &len, STDIN_FILENO);

		++msh->cmd_count; /* keep track of the number of inputs to the shell */

		/* check for empty prompt or if Ctrl+D was received */
		if (n_read == 0)
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			handle_exit(msh, multi_free); /* clean up and leave */
		}

		msh->exit_code = parse_line(msh);
		safe_free(msh->line);
	}

	return (msh->exit_code);
}
