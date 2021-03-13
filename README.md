# John Chandara’s Shell 352

This project consists of designing a C program to serve as a shell interface that accepts user commands and then executes each command in a separate process. Your mplementation will support input and output redirection, pipes, and background processes. Completing this project will involve using the Linux `fork()`, `exec()`, `wait()`, `waitpid()`, `pipe()` and `dup2()` system calls. It must be tested on pyrite.

## Supported commands

Commands exist in two forms: pre-compiled commands and external commands.

### Pre-compiled commands

* cd - change directory
* exit/quit - leave the shell
* bg - partially supported
* fg - partially supported
* jobs - partially supported

### External commands

All external commands are fully functional!

## The Buongiorno Library

This library is developed exclusively for the 352 shell and contains a plethora of parsing and grammar-related tooling dedicated for CLI interfacing.