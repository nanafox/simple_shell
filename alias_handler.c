#include "shell.h"

/**
 * handle_alias - handles the processing of alias
 * @head: a pointer to the list containing all aliases
 * @command: the list of commands containing alias-specific lines
 *
 * Return: 0 on success, -1 on error
 */
int handle_alias(alias_t **head, char **command)
{
	char *value, *loc, *name, *tmp;
	ssize_t i, offset, exit_code = 0;

	if (!_strcmp("unalias", command[0]))
		return (unalias(head, command));
	if ((!_strcmp(command[0], "alias")  && command[1] == NULL))
		print_aliases(*head);
	for (i = 1; command[i] != NULL; i++)
	{
		tmp = _strdup(command[i]);
		loc = _strchr(tmp, '=');
		if (loc != NULL)
		{
			offset = (&loc[0]) - (&tmp[0]);
			tmp[offset] = '\0';
			name = _strdup(tmp);
			if (name == NULL)
			{
				safe_free(tmp);
				return (-1);
			}
			value = (tmp[offset + 1] == '"' || tmp[offset + 1] == '\'')
				? extract_value(&tmp[offset + 1]) : _strdup(&tmp[offset + 1]);
			if (value == NULL)
			{
				multi_free("ss", value, name);
				return (-1);
			}
			if (add_alias(head, name, value) == NULL)
			{
				multi_free("ss", value, name);
				return (-1);
			}
			multi_free("ss", value, name);
		}
		else
			exit_code = print_alias(*head, command[i]);
		safe_free(tmp);
	}
	return (exit_code);
}

/**
 * extract_value - returns the string value enclosed in quotes
 * @value: the string containing value
 *
 * Return: the string data without the quotes
 */
char *extract_value(const char *value)
{
	int i, j;
	char quote = value[0];
	int length = strlen(value);
	char *content = malloc(length);

	if (content == NULL)
	{
		return (NULL);
	}

	for (i = 1, j = 0; i < length && value[i] != quote; ++i, ++j)
	{
		content[j] = value[i];
	}
	content[j] = '\0';

	return (content);
}
