#include "shell.h"

static int exit_code; /* keeps track of all exit codes */
static alias_t *aliases;

/**
 * parse_line - parses the receive command line, processes it before handing it
 * over to the executor only after it has confirmed the command is valid
 * @line: the command line received
 * @path_list: a list of pathnames in the PATH variable
 *
 * Return: the exit code of the executed program, else -1 if something goes
 * wrong
 */
int parse_line(char *line, path_t *path_list)
{
	char **commands = NULL;

	/* skip normal ENTER keys and leading comments */
	if (*line == '\n' || *line == '#')
		return (0);

	/* first of all, let's get rid of all comments */
	line = handle_comments(line);

	/* now let's all the commands provided by the user */
	commands = _strtok(line, "\n;");
	if (commands == NULL)
	{
		perror("_strtok");
		return (-1); /* an error occurred while getting the commands */
	}

	exit_code = parse(commands, path_list, line);
	free_str(commands);
	return (exit_code);
}

/**
 * parse - parses an array of commands received from the prompt
 * @commands: an array of command line strings
 * @path_list: a list of pathnames in the PATH variable
 * @line: the command line received
 *
 * Return: the exit code of the executed program
 */
int parse(char **commands, path_t *path_list, char *line)
{
	ssize_t i, offset;
	char **sub_command = NULL, *cmd = NULL, *operator = NULL;
	char *next_cmd = NULL, *temp_next_cmd = NULL;

	for (i = 0; commands[i] != NULL; i++)
	{
		operator = get_operator(commands[i]);
		if (operator != NULL)
		{
			offset = strcspn(commands[i], operator);
			/* extract the command before the operator */
			cmd = strndup(commands[i], offset);
			if (cmd == NULL)
				return (0);
			sub_command = _strtok(cmd, NULL);
			safe_free(cmd);
			if (sub_command == NULL)
				return (0);
			sub_command = handle_variables(sub_command, exit_code);
			parse_helper(commands, sub_command, path_list, line, i);

			temp_next_cmd = _strdup(&commands[i][offset + 2]);
			safe_free(next_cmd);
			safe_free(commands[i]);

			/* check the exit code and react accordingly */
			if ((!_strcmp(operator, "&&") && exit_code == 0) ||
				(!_strcmp(operator, "||") && exit_code != 0))
			{
				commands[i] = temp_next_cmd;
				parse(commands, path_list, line);
				next_cmd = temp_next_cmd;
			}
			else
				safe_free(temp_next_cmd);
		}
		else
			parse_and_execute(commands, commands[i], path_list, line, i);
	}
	return (exit_code);
}

/**
 * parse_and_execute - parses each sub command line and executes it
 * @commands: an array of command line strings
 * @path_list: a list of pathnames in the PATH variable
 * @cur_cmd: the current command in the commands array
 * @line: the command line received
 * @index: the current index in commands array
 *
 * Return: the exit code of the executed program
 */
int parse_and_execute(char **commands, char *cur_cmd, path_t *path_list,
					  char *line, size_t index)
{
	char **sub_command = NULL;

	/* get the sub commands and work on them */
	sub_command = _strtok(cur_cmd, NULL);
	if (sub_command == NULL)
	{
		return (0); /* probably just lots of tabs or spaces, maybe both */
	}

	/* check for variables */
	sub_command = handle_variables(sub_command, exit_code);
	parse_helper(commands, sub_command, path_list, line, index);

	/* cleanup and leave */
	safe_free(commands[index]);
	return (exit_code);
}

/**
 * parse_helper - performs extra parsing on behalf of the parse and execute
 * function
 * @commands: an array of commands received on the command line
 * @sub_command: an array of sub commands generated from the commands array
 * @path_list: a list of PATH directories
 * @line: the actual line the user typed on the prompt
 * @index: the current index in commands array
 */
void parse_helper(char **commands, char **sub_command, path_t *path_list,
				  char *line, size_t index)
{
	char *alias_value;

	if (!_strcmp(sub_command[0], "alias") ||
		!_strcmp(sub_command[0], "unalias"))
	{
		exit_code = handle_alias(&aliases, sub_command);
		free_str(sub_command);
		return;
	}

	alias_value = get_alias(aliases, sub_command[0]);
	if (alias_value != NULL)
	{
		safe_free(sub_command[0]);
		sub_command[0] = _strdup(alias_value);
		safe_free(alias_value);
	}
	exit_code = handle_builtin(sub_command, commands, line, aliases, path_list,
							   exit_code);
	if (exit_code != NOT_BUILTIN)
	{
		free_str(sub_command);
		return; /* shell builtin executed well */
	}

	if (path_list != NULL) /* handle the command with the PATH variable */
	{
		exit_code = handle_with_path(path_list, sub_command);
		if (exit_code == -1)
			exit_code = print_cmd_not_found(sub_command, commands, index);
		free_str(sub_command);
	}
	else
	{
		if (access(sub_command[0], X_OK) == 0 && _strchr(sub_command[0], '/'))
			exit_code = execute_command(sub_command[0], sub_command);
		else
			exit_code = print_cmd_not_found(sub_command, commands, index);
		free_str(sub_command);
	}
}

/**
 * print_cmd_not_found - prints the command not found error
 * @sub_command: the actual command executed
 * @commands: a list of commands received on the command line
 * @index: current index in the commands list
 *
 * Return: 127 command not found code, else 0
 */
int print_cmd_not_found(char **sub_command, char **commands, size_t index)
{
	static size_t err_count = 1;

	dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", _getenv("_"), err_count,
			sub_command[0]);
	err_count++;

	if (commands[index + 1] == NULL)
	{
		return (CMD_NOT_FOUND); /* command not found */
	}

	return (0);
}
