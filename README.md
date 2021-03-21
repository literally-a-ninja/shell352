# John Chandara’s Shell 352

This project consists of designing a C program to serve as a shell interface that accepts user commands and then executes each command in a separate process. Your mplementation will support input and output redirection, pipes, and background processes. Completing this project will involve using the Linux `fork()`, `exec()`, `wait()`, `waitpid()`, `pipe()` and `dup2()` system calls. It must be tested on pyrite.

## How to compile

1. Ensure your environment has `pthreads`, `cmake` and `make`.
2. Run `cmake .` in the root working directory to generate the Makefile
3. Run `make` for build the program.

$$
\usepackage{tikz-cd}
\begin{tikzcd}
A \arrow[d, "g"] \arrow[r, "f"] & B \arrow[r, "\alpha"] \arrow[d, "\gamma"] & D \arrow[d, "\beta"] \\
C \arrow[rru, "h"] & B' \arrow[r, "\lambda"] & D'
\end{tikzcd}
$$



## Supported commands

Commands exist in two forms: pre-compiled commands and external commands.

NOTE: Due to a lack of time at the latter portion of the project (see git history), jobs are NOT FULLY IMPLEMENTED at the moment. That being said, processes may be launched in either foreground or background mode with `SIGSTOP` interrupts fully implemented at the time being.

### Pre-compiled commands

* cd - change directory
* exit/quit - leave the shell
* bg - partially supported
* fg - partially supported
* jobs - partially supported

### External commands

All external commands are fully functional!

## The Buongiorno Library

This library is developed exclusively for the 352 shell and contains a plethora of parsing and grammar-related tooling dedicated for CLI interfacing.dr