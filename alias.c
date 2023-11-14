#include "shell.h"

/**
 * add_alias - adds an alias to the linked list
 * @head: a pointer to the list containing the list of aliases
 * @name: the name of the alias to add
 * @value: the value associated with the name
 *
 * Return: the address of the newly added node (alias),
 * or the address of a previous if it already exists
 */
alias_t *add_alias(alias_t **head, const char *name, const char *value)
{
	alias_t *current, *alias;

	alias = malloc(sizeof(alias_t));
	if (alias == NULL)
		return (NULL);

	alias->name = _strdup(name);
	alias->value = _strdup(value);
	alias->next = NULL;

	if (*head == NULL)
	{
		(*head) = alias;
		return (*head);
	}

	current = *head;
	while (current != NULL)
	{
		if (!_strcmp(name, current->name))
		{
			safe_free(current->value);
			current->value = _strdup(value);
			safe_free(alias->name);
			safe_free(alias->value);
			safe_free(alias);
			return (current);
		}
		current = current->next;
	}

	current = *head;
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = alias;

	return (alias);
}

/**
 * print_aliases - prints all the aliases
 * @head: the list of aliases
 */
void print_aliases(const alias_t *head)
{
	if (head == NULL)
		return;

	while (head != NULL)
	{
		printf("%s='%s'\n", head->name, head->value);
		head = head->next;
	}
}

/**
 * unalias - removes an alias from the list
 * @head: a pointer to the list of aliases
 * @command: a null-terminated array of aliases to remove
 *
 * Return: 0 if alias was found and removed successfully, else 1
 */
int unalias(alias_t **head, char **command)
{
	alias_t *current, *prev;
	size_t i;
	char *name;
	int exit_code = 1; /* assume failure by default */

	current = *head;
	for (i = 1; command[i] != NULL; i++)
	{
		name = strtok(command[i], "=");
		while (current != NULL)
		{
			if (!_strcmp(current->name, name))
			{
				if (current == *head)
					*head = (*head)->next;
				else
					prev->next = current->next;
				multi_free("ss", current->name, current->value);
				safe_free(current);
				exit_code = 0;
				break;
			}
			prev = current;
			current = current->next;
		}
		current = *head; /* point back to head before next run */

		if (exit_code != 0)
		{
			dprintf(2, "unalias: %s not found\n", name);
			exit_code = 1;
		}
	}
	return (exit_code);
}

/**
 * print_alias - prints a specific alias based on the name given
 * @head: the list of aliases
 * @name: the name of the alias to print
 *
 * Return: 0 if alias was found, else 1
 */
int print_alias(const alias_t *head, const char *name)
{
	if (head == NULL)
	{
		dprintf(2, "alias: %s not found\n", name);
		return (1); /* the list is empty, can't search */
	}

	while (head != NULL)
	{
		if (!_strcmp(head->name, name))
		{
			printf("%s='%s'\n", head->name, head->value);
			return (0);
		}
		head = head->next;
	}

	/* we reached the end of the list and didn't find the specified alias */
	dprintf(2, "alias: %s not found\n", name);
	return (1);
}

/**
 * get_alias - returns the value of an alias
 * @head: the list containing aliases
 * @name: the name of the alias
 *
 * Return: the value of the alias, else NULL if not found
 */
char *get_alias(alias_t *head, const char *name)
{
	char *value, *chained_value;
	alias_t *current;

	if (head == NULL || name == NULL)
		return (NULL); /* head is empty or name provided is invalid */

	current = head;

	while (current != NULL)
	{
		if (!_strcmp(current->name, name))
		{
			value = _strdup(current->value);
			if (value == NULL)
				return (NULL);
			if (!_strcmp(current->name, current->value))
				return (value);

			/* check for aliases chained to other aliases */
			chained_value = get_alias(head, value);
			if (chained_value != NULL)
			{
				safe_free(value);
				return (chained_value);
			}

			return (value); /* not a chained value, return as-is */
		}
		current = current->next;
	}

	return (NULL);
}
