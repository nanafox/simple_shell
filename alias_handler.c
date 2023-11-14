#include "shell.h"

static int exit_code;

/**
 * handle_alias - handles the processing of alias
 * @head: a pointer to the list containing all aliases
 * @command: the list of commands containing alias-specific lines
 *
 * Return: 0 on success, -1 on error
 */
int handle_alias(alias_t **head, char *command)
{
	if (_strlen(command) == 5)
		print_aliases(*head);

	else if (!_strncmp(command, "alias", 5))
	{
		if (!_strchr(command, '='))
			process_non_matching(*head, command + 5, 1);
		else
			parse_aliases(command, head);
	}

	else if (!_strncmp(command, "unalias", 7))
		return (unalias(head, command));

	return (exit_code);
}

/**
 * parse_aliases - extract aliases from the input string using regular
 * expressions.
 * @input: the input string containing aliases
 * @aliases: a pointer to a list of aliases
 *
 * Description: This function uses regular expressions to extract aliases from
 * the input string. It populates the provided array of Alias structures with
 * the parsed aliases and updates the aliasCount accordingly.
 */
void parse_aliases(const char *input, alias_t **aliases)
{
	char *input_ptr = NULL;
	size_t valueLength, alias_count = 0;
	regmatch_t matches[3];
	regex_t regex;
	const char *pattern =
		"([^\"]\\S*|\"[^\"]*\"|'[^\']*')=([^\"]\\S*|\"[^\"]*\"|'[^\']*')";

	if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
		return; /* regular expression compilation failed */
	input_ptr = (char *)input;
	while (regexec(&regex, input_ptr, 3, matches, 0) == 0)
	{
		char name[MAX_ALIAS_LENGTH] = {0};
		char value[MAX_VALUE_LENGTH] = {0};

		/* extract the alias name, accounts for the leading spaces */
		_strncpy(name, (input_ptr + 1) + matches[1].rm_so,
				 (matches[1].rm_eo - 1) - matches[1].rm_so);
		_strncpy(value, input_ptr + matches[2].rm_so,
				 matches[2].rm_eo - matches[2].rm_so); /* extract the alias value */
		name[matches[1].rm_eo - matches[1].rm_so] = '\0';

		valueLength = matches[2].rm_eo - matches[2].rm_so;
		if (isquote(value[0])) /* remove quotes */
		{
			_strncpy(value, input_ptr + matches[2].rm_so + 1, valueLength - 2);
			value[valueLength - 2] = '\0';
		}
		else /* not enclosed in quotes, copy as is */
			value[valueLength] = '\0';
		if (add_alias(aliases, name, value) == NULL)
			return;
		if (alias_count)
			process_non_matching(*aliases, input_ptr, 0);
		alias_count++;					/* increment alias count */
		input_ptr += matches[0].rm_eo; /* keep searching */
	}
	if (alias_count)
		process_non_matching(*aliases, input_ptr, 1);
	regfree(&regex);
}

/**
 * process_non_matching - processes strings that do not have the `name=value`
 * format while parsing aliases
 * @aliases: a list containing aliases
 * @non_matching: the string to check for non-matching patterns
 * @end: used to signal the end. 1 means it can process the whole string at
 * once. 0 means it can do only word token at a time
 */
void process_non_matching(alias_t *aliases, const char *non_matching, int end)
{
	char *token, *dup;

	if (non_matching == NULL || *non_matching == '\0')
		return; /* there's nothing to work on, everything matched */

	dup = _strdup(non_matching);
	token = strtok(dup, " ");

	/* it is non-matching if it doesn't contain an equal sign */
	if (!_strchr(token, '='))
	{
		if (end)
		{
			while (token != NULL)
			{
				exit_code = print_alias(aliases, token);
				token = strtok(NULL, " ");
			}
		}
		else
			exit_code = print_alias(aliases, token);
	}
	free(dup);
}

/**
 * build_alias_cmd - builds the correct command line when the received input is
 * a valid alias command
 * @sub_command: a pointer to the array containing the commands
 * @alias_value: the value of the alias command
 */
void build_alias_cmd(char ***sub_command, char *alias_value)
{
	char **dup_array = NULL;

	if ((*sub_command)[1] != NULL)
	{
		/* save a copy of the the commands array, excluding the alias */
		dup_array = duplicate_str_array((*sub_command) + 1);

		/* memory and build the command line string based on the alias value */
		free_str(*sub_command);
		*sub_command = _strtok(alias_value, NULL);

		/* concatenate both arrays to form a complete command string */
		concatenate_arrays(sub_command, dup_array);

		/* clean up and return */
		free_str(dup_array);
	}
	else
	{
		/* there was alias alright but no other arguments */
		free_str(*sub_command);
		*sub_command = _strtok(alias_value, NULL);
	}
}
