#include "shell.h"

/**
 * handle_comments - parses a command received on the shell and handles the '#'
 * symbol (comments) if any.
 * @command: the command line string
 *
 * Description: The shell doesn't need to process comments so the job of this
 * function is to parse the command string while search for the '#' character.
 * Once it finds one, it re-terminates the original command string to exclude
 * everything from the '#' character to the end. Essentially removing the
 * comments before the command is passed to other functions which handle the
 * execution, or further processing if required.
 *
 * Return: the updated (if needed) command string ready for execution or for
 * further processing.
 */
char *handle_comments(char *command)
{
	int offset;
	char *loc = _strchr(command, '#');

	if (loc == NULL)
		return (command); /* does not contain a comment */

	/* let's get the index where this bad boy exists */
	offset = (&loc[0]) - (&command[0]);

	/* check for when a '#' was found but it's not the start of a comment */
	if (!isspace(command[offset - 1]))
	{
		return (command); /* return the command as received, don't touch it */
	}
	else
	{
		/* yeah! that's a comment, handle the termination properly */
		command[offset - 1] = '\0';
	}

	return (command);
}
