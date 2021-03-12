#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin.c"
#include "exec.h"
#include "jobs.c"
#include "jobs.h"
#include "shell352.h"

void applyRedirects (cmd_t *cmd)
{
    int fd;
    if (cmd->props->redirect_to)
    {
        fd = open (cmd->props->redirect_to->dest, O_RDWR | O_CREAT,
                   S_IRWXG | S_IRGRP);
        dup2 (fd, STDOUT_FILENO);
        dup2 (fd, STDERR_FILENO);
        close (fd);
    }

    if (cmd->props->redirect_from)
    {
        fd = open (cmd->props->redirect_to->dest, O_RDWR, S_IRWXG | S_IWGRP);

        dup2 (STDIN_FILENO, fd);
        close (fd);
    }
}

int assertSpawn (int fdOut, int procId)
{
    if (fdOut < 0)
    {
        fprintf (stderr, "Failed to create pipes!");
        return 1;
    }

    if (procId < 0)
    {
        fprintf (stderr, "Fork Failed");
        return 1;
    }

    return 0;
}

int exec_or_fail (cmd_t *cmd)
{
    builtin_callback_t *cb;
    if ((cb = findBuiltinCmd (cmd->executable)))
    {
        return cb (cmd, g_env);
    }

    char *fullPath;
    if (!(fullPath = file_resolve (cmd->executable)))
    {
        fprintf (stderr, "%s: command not found.\n", cmd->executable);
        return 1;
    }

    execvp (fullPath, cmd->args);
    free (fullPath);

    return 0;
}

int spawn (run_t *run, unsigned depth, int o [2])
{
    // If we still have commands to process first.
    if (depth > 0)
    {
        int o2 [2];

        // Ensure that pipes and our child exists.
        if (pipe (o2) < 0)
            exit (1);

        // Have the child spawn more if necessary, should self-termiate after.
        if (!fork ())
            spawn (run, depth - 1, o2);

        // Wait for child exit and read what they left behind.
        close (STDIN_FILENO);
        dup2 (o2 [IREAD], STDIN_FILENO);
        wait (NULL);
        close (o2 [IWRITE]);
    }

    cmd_t *cmd = run->commands [depth];

    // Configure our pipes correctly and grab our command.
    close (STDOUT_FILENO);
    close (o [IREAD]);
    dup2 (o [IWRITE], STDOUT_FILENO);
    close (o [IWRITE]);

    // I'm not sure if this will interfere with out pipes
    // But if our pipes fail this is why.
    applyRedirects (cmd);

    // Run our command
    if (exec_or_fail (cmd))
        exit (1);

    exit (0);
}

int execute_runs (run_t *run)
{
    // Ensure that our pipe exists
    int o [2];
    if (pipe (o) < 0)
        exit (1);

    // FORK after PIPE to avoid issues!
    switch (fork ())
    {
    case 0:
        spawn (run, run->commands_size - 1, o);
        break;

    default:
        wait (NULL);
        close (o [IWRITE]);

        char c [1];
        while (read (o [IREAD], &c, sizeof (char)))
            write (STDOUT_FILENO, &c, sizeof (char));

        close (o [IREAD]);
        break;
    }

    return 0;
}
