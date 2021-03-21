#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin.c"
#include "exec.h"
#include "jobs.h"
#include "shell352.h"

void exec_safe_attach_redirects (cmd_t *cmd)
{
    if (cmd->props->redirect_from)
    {
        int fd;
        char *absPath;
        absPath = path_resolve_relative (
            cmd->props->redirect_from->destination,
            B_strlen (cmd->props->redirect_from->destination), g_env->m_ptrWd,
            B_strlen (g_env->m_ptrWd));

        if ((fd = open (absPath, O_RDONLY)) < 0)
        {
            fprintf (stderr, "E: An error has occured while attempting to "
                             "redirect stdin to some input.\n");
            perror ("open: ");
            free (absPath);
            exit (-1);
        }

        close (STDIN_FILENO);
        dup2 (fd, STDIN_FILENO);
        close (fd);
        free (absPath);
    }

    if (cmd->props->redirect_to)
    {
        int fd;
        char *absPath;
        absPath = path_resolve_relative (
            cmd->props->redirect_to->destination,
            B_strlen (cmd->props->redirect_to->destination), g_env->m_ptrWd,
            B_strlen (g_env->m_ptrWd));

        if ((fd = open (absPath, O_WRONLY | O_CREAT,
                        S_IRWXU | S_IRGRP | S_IROTH)) < 0)
        {
            fprintf (stderr, "E: An error has occured while attempting to "
                             "redirect stdout to some output.\n");
            perror ("open: ");
            free (absPath);
            exit (-1);
        }

        dup2 (fd, STDOUT_FILENO);
        dup2 (fd, STDERR_FILENO);
        close (fd);
        free (absPath);
    }
}

int exec_safe_execvp (cmd_t *cmd)
{
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

int exec_safe_spawn (job_t *job, unsigned depth, int o [2])
{
    run_t *run = job->run;

    // If we still have commands to process first.
    if (depth > 0)
    {
        int o2 [2];

        // Ensure that pipes and our child exists.
        if (pipe (o2) < 0)
            exit (1);

        // Have the child spawn more if necessary, should self-termiate after.
        switch (fork ())
        {
        case 0:
            exec_safe_spawn (job, depth - 1, o2);
            break;
        case -1:
            exit (RETURNVAL_FATAL_ERROR);
            break;
        default:
            break;
        }

        // Wait for child exit and read what they left behind.
        dup2 (o2 [IREAD], STDIN_FILENO);
        int wstatus;
        wait (&wstatus);

        close (o2 [IWRITE]);
    }

    cmd_t *cmd = run->commands [depth];

    // Configure our pipes correctly and grab our command.
    close (STDOUT_FILENO);
    close (o [IREAD]);
    dup2 (o [IWRITE], STDOUT_FILENO);
    close (o [IWRITE]);

    // Attach redirects after, redirects override pipes.
    exec_safe_attach_redirects (cmd);

    // Run our command
    if (exec_safe_execvp (cmd))
        exit (1);

    exit (0);
}

void exec_safe_handle_returnval (int *wstatus)
{
    if (!WIFEXITED (*wstatus))
        return;

    // If non-zero return status, abort.
    int returnVal;
    if ((returnVal = WEXITSTATUS (*wstatus)))
        exit (returnVal);
}

void exec_handle_returnval (int *wstatus)
{
    if (!WIFEXITED (*wstatus))
        return;

    switch (WEXITSTATUS (*wstatus))
    {
    /* Something terrible occured, kill
     * parent to prevent further damage.*/
    case RETURNVAL_FATAL_ERROR:
        g_shellStatus |= SHELL_DIE;
        break;
    }
}

job_t *exec (run_t *run)
{
    if (!run->commands_size)
        return 0;

    cmd_t *firstCmd = run->commands [0];
    builtin_callback_t *cb;
    if ((cb = findBuiltinCmd (firstCmd->executable)))
    {
        cb (firstCmd, g_env);
        return 0;
    }

    job_t *job = ctor_job_t (run);

    job->id          = job_find_next_available_id ();
    g_jobs [job->id] = job;
    g_fgJob          = job;
    g_job_count++;

    // FORK after PIPE to avoid issues!
    pid_t pid;
    char c;
    switch (pid = fork ())
    {
    case 0:
        // We set this at the very last stage to avoid race conditions.
        job->state = Running;
        exec_safe_spawn (job, run->commands_size - 1, job->pipe);
        break;
    case -1:
        fprintf (stderr, "FE: Cannot fork any more child processes. Aborting.");
        g_shellStatus |= SHELL_DIE;
        break;
    default:
        job->pid = pid;
        break;
    }

    return job;
}
