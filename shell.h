#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_ARGS 100

/* Function prototypes */
void execute_command(char **args, char **environ);
char **parse_input(char *input);
char *search_path(const char *command);
void execute_exit(char **args);
void print_env(char **envp);
void execute_command_or_builtin(char **args, char **environ);
void handle_input(char **input, size_t *len, ssize_t *nread);

#endif /* SHELL_H */
