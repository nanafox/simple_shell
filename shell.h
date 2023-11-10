#ifndef SHELL_H
#define SHELL_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>

/* macros */

#define BUFF_SIZE 1024
#define SPACE ' '
#define CMD_NOT_FOUND 127
#define PROMPT_SIZE 4096
#define PATH_SIZE 2048
#define NOT_BUILTIN 18
#define RUNNING 1
#define CMD_ERR 2

/* function macros */

#define isspace(c) ((c) == SPACE)
#define isdigit(c) (((c) >= '0' && (c) <= '9')) /* checks for digits */
#define isalpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define isnegative(c) (((c) == '-') ? -1 : 1)
#define issign(c) ((c) == '-' || (c) == '+')

/* string handlers */

size_t _strlen(const char *s);
char *_strdup(const char *str);
char *_strchr(const char *s, int c);
char *_strrchr(const char *s, int c);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);
int _strcmp(const char *s1, const char *s2);
char *_strpbrk(const char *s, const char *accept);
size_t _strspn(const char *s, const char *accept);
char **_strtok(const char *str, const char *delim);
char *_strncpy(char *dest, const char *src, size_t n);
int _strncmp(const char *s1, const char *s2, size_t n);
int get_word_count(const char *str, const char *delim);
char *_strstr(const char *haystack, const char *needle);

/* memory handlers */

void free_str(char **str_array);
char *new_word(const char *str, int start, int end);
void *_memcpy(void *dest, const void *src, size_t n);
void *_realloc(void *old_mem_blk, size_t old_size, size_t new_size);

/* a safer way to deallocate dynamic memory */
void _free(void **ptr);
#define safe_free(p) _free((void **)&(p)) /* _free's frontend */

/* a custom implementation of the getline function */
ssize_t _getline(char **lineptr, size_t *n, int fd);

/* shows the prompt in interactive mode */
void show_prompt(void);

/* environment and PATH handlers */

extern char **environ;

/**
 * struct _path - builds a linked list of the PATH variable in an environment
 * variable
 * @pathname: each path name in the PATH variable
 * @next: a pointer to the next node
 */
typedef struct _path
{
	char *pathname;
	struct _path *next;
} path_t;

void _printenv(void);
void free_list(path_t **head);
char *_getenv(const char *name);
path_t *build_path(path_t **head);
void print_path(path_t *list);

/* parsers and executors */

char *handle_comments(char *command);
int parse_line(char *line, path_t *path_list);
int execute_command(char *pathname, char *argv[]);
int parse_and_execute(char **commands, path_t *path_list, char *line);
int handle_with_path(path_t *path_list, char **sub_command);
int print_cmd_not_found(char **sub_command, char **commands, size_t index);
int handle_file_as_input(char *filename, path_t *path_list);
char **handle_variables(char **commands, int exit_code);
void _free_on_exit(const char *format, ...);

/* numbers */

void _reverse(char *buffer, size_t len);
void _itoa(size_t n, char *s);
int _atoi(const char *s);

/* builtin handlers */

int handle_cd(const char *pathname);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);
int handle_builtin(char **sub_command, char **commands, path_t *path_list,
		char *line, int exit_code);
int handle_exit(char *exit_code, int status,
		void (*cleanup)(const char *format, ...),
		char **sub_command, char **commands, path_t **path_list, char *line);

#endif /* SHELL_H */
