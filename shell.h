#ifndef SHELL_H
#define SHELL_H

extern char **environ;

#define MAX_ARGS 1024
#define INIT_BUF_SIZE 64 /* Initial buffer size */
#define STATIC_BUF_SIZE 1024     /* Static buffer size */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Function prototypes */
int execute_command(char **args, char **envp, char *program_name,
		int line_number, int last_status);
char **parse_input(char *input);
char *search_path(const char *command);
void execute_exit(char **args, int last_status);
void print_env(char **envp);
int execute_command_or_builtin(char **args, char **envp,
		char *program_name, int line_number);
void handle_input(char **input, size_t *len, ssize_t *nread,
		char **argv, int line_number);
ssize_t custom_getline(char **lineptr, size_t *n);
char *custom_strdup(const char *str);
char *custom_strtok(char *str, const char *delim);
void execute_setenv(char **args);
void execute_unsetenv(char **args);
int handle_builtins(char **args, char **envp, char *program_name,
		int line_number);
int execute_cd(char **args);
char **parse_commands(char *input);
void execute_commands(char *input, char **environ, char *program_name,
		int line_number);
void handle_alias(char **args);
void print_all_aliases(void);
void print_alias(const char *name);
void remove_alias(const char *name);
void add_alias(const char *name, const char *value);
void free_alias_list(void);
void execute_commands_with_separator(char *input,
		char **argv, int line_number);
void handle_env_assignment(char *input);
int simple_shell(void);
/* Prototypes for new helper functions */
void handle_external_command(char **args, char **environ,
		char *program_name, int line_number);
int handle_builtin(char **args, char **environ,
		char *program_name, int line_number);
void handle_subcommands(char *trimmed_command,
		char **environ, char *program_name, int line_number);

#endif /* SHELL_H */
