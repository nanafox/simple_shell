#include "shell.h"

/**
 * parse_line - parses the receive command line, processes it before handing it
 * over to the executor only after it has confirmed the command is valid
 * @msh: contains all the data relevant to the shell's operation
 *
 * Return: the exit code of the executed program, else -1 if something goes
 * wrong
 */
int parse_line(shell_t *msh)
{
	size_t i;

	/* skip normal ENTER keys and leading comments */
	if (*msh->line == '\n' || *msh->line == '#')
		return (0);

	/* first of all, let's get rid of all comments */
	msh->line = handle_comments(msh->line);

	msh->tokens = _strtok(msh->line, "\n");
	if (msh->tokens == NULL)
	{
		fprintf(stderr, "Not enough system memory to continue\n");
		return (-1);
	}

	for (i = 0; msh->tokens[i] != NULL; i++)
	{
		msh->token = msh->tokens[i];

		if (!_strcmp(msh->tokens[i], "exit") && msh->tokens[i + 1] == NULL)
		{
			handle_exit(msh, multi_free);
		}

		/* now let's tokenize all the commands provided by the user */
		msh->commands = _strtok(msh->token, ";\n");
		if (msh->commands == NULL)
		{
			fprintf(stderr, "Memory allocation failed...\n");
			return (-1);
		}

		msh->exit_code = parse(msh);
		free_str(&msh->commands);
	}

	free_str(&msh->tokens);
	return (msh->exit_code);
}

/**
 * parse - parses an array of commands received from the prompt
 * @msh: contains all the data relevant to the shell's operation
 *
 * Return: the exit code of the executed program
 */
int parse(shell_t *msh)
{
	ssize_t i, offset;
	char *cur_cmd = NULL, *operator = NULL;
	char *next_cmd = NULL, *temp_next_cmd = NULL;

	for (i = 0; msh->commands[i] != NULL; i++)
	{
		operator = get_operator(msh->commands[i]);
		if (operator != NULL)
		{
			offset = strcspn(msh->commands[i], operator);
			/* extract the command before the operator */
			cur_cmd = strndup(msh->commands[i], offset);
			if (cur_cmd == NULL)
				return (0);
			msh->sub_command = _strtok(cur_cmd, NULL);
			safe_free(cur_cmd);
			if (msh->sub_command == NULL)
				return (0);
			msh->sub_command = handle_variables(msh);
			parse_helper(msh, i);

			temp_next_cmd = _strdup(&msh->commands[i][offset + 2]);
			safe_free(next_cmd);
			safe_free(msh->commands[i]);

			/* check the exit code and react accordingly */
			if ((!_strcmp(operator, "&&") && msh->exit_code == 0) ||
				(!_strcmp(operator, "||") && msh->exit_code != 0))
			{
				msh->commands[i] = temp_next_cmd;
				parse(msh);
				next_cmd = temp_next_cmd;
			}
			else
				safe_free(temp_next_cmd);
		}
		else
			parse_and_execute(msh, i);
	}
	return (msh->exit_code);
}

/**
 * parse_and_execute - parses each sub command line and executes it
 * @msh: contains all the data relevant to the shell's operation
 * @index: the current index in commands array
 *
 * Return: the exit code of the executed program
 */
int parse_and_execute(shell_t *msh, size_t index)
{
	/* get the sub commands and work on them */
	msh->sub_command = _strtok(msh->commands[index], NULL);
	if (msh->sub_command == NULL)
	{
		return (0); /* probably just lots of tabs or spaces, maybe both */
	}

	/* check for variables */
	msh->sub_command = handle_variables(msh);
	if (msh->sub_command[0] != NULL && msh->sub_command != NULL)
		parse_helper(msh, index);
	else
		free_str(&msh->sub_command);

	/* cleanup and leave */
	safe_free(msh->commands[index]);
	return (msh->exit_code);
}

/**
 * parse_helper - performs extra parsing on behalf of the parse and execute
 * function
 * @msh: contains all the data relevant to the shell's operation
 * @index: the current index in commands array
 */
void parse_helper(shell_t *msh, size_t index)
{
	char *alias_value;

	if (!_strcmp(msh->sub_command[0], "alias") ||
			!_strcmp(msh->sub_command[0], "unalias"))
	{
		msh->exit_code = handle_alias(&msh->aliases, msh->commands[index]);
		free_str(&msh->sub_command);
		return;
	}

	alias_value = get_alias(msh->aliases, msh->sub_command[0]);
	if (alias_value != NULL)
	{
		build_alias_cmd(&msh->sub_command, alias_value);
		safe_free(alias_value);
	}

	msh->exit_code = handle_builtin(msh);
	if (msh->exit_code != NOT_BUILTIN)
	{
		free_str(&msh->sub_command);
		return; /* shell builtin executed well */
	}
	/* handle the command with the PATH variable */
	if (msh->path_list != NULL && !_strchr(msh->sub_command[0], '/'))
	{
		msh->exit_code = handle_with_path(msh);
		if (msh->exit_code == -1)
			msh->exit_code = print_cmd_not_found(msh);
	}
	else
	{
		if (access(msh->sub_command[0], X_OK) == 0 &&
				_strchr(msh->sub_command[0], '/'))
			msh->exit_code = execute_command(msh->sub_command[0], msh);
		else
			msh->exit_code = print_cmd_not_found(msh);
	}
	free_str(&msh->sub_command);
}

/**
 * print_cmd_not_found - prints the command not found error
 * @msh: contains all the data relevant to the shell's operation
 *
 * Return: 127 command not found code, else 0
 */
int print_cmd_not_found(shell_t *msh)
{
	dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", msh->prog_name,
			msh->cmd_count, msh->sub_command[0]);

	return (CMD_NOT_FOUND); /* command not found */
}
