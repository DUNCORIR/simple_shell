0x16. C - Simple Shell
Overview
The Simple Shell is a UNIX command-line interpreter that mimics the behavior of a basic shell. It handles command execution, supports built-in commands, and manages environment variables. This shell provides a foundation for learning and extending shell functionality.

Features
1.Command Execution: Executes commands with arguments.

2.PATH Handling: Searches for commands in directories listed in the PATH environment variable.

3.Built-in Commands:

exit: Exits the shell with an optional status code.
env: Prints the current environment variables.
setenv: Sets or modifies environment variables.
unsetenv: Removes environment variables.
cd: Changes the current working directory.
alias: Defines and manages command aliases.
4.Advanced Features:

Custom getline implementation for reading input.
Support for logical operators (&& and ||) and command separators (;).
Variable replacement and handling special variables ($?, $$).
Comment handling (#).
File input mode to execute commands from a file.
