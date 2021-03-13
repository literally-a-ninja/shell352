#include <stdlib.h>
#include <unistd.h>

#include "jobs.h"
#include "shell352.h"

job_t *ctor_job_t (run_t *run)
{
    unsigned id;
    if ((id = job_find_next_available_id ()) < 0)
    {
        fprintf (stderr,
                 "FE: Cannot create anymore jobs! Exceeded capacity.\n");
        g_shellStatus |= SHELL_DIE;
    }

    job_t *job      = malloc (sizeof (job_t));
    job->run        = run;
    job->background = 0;
    job->state      = Init;
    job->id         = id;

    if (pipe (job->pipe) < 0)
    {
        fprintf (stderr,
                 "FE: Could not create necessary pipes for new job. Aborting.");
        g_shellStatus |= SHELL_DIE;
    }

    if (run->commands_size)
    {
        cmd_t *lastCmd  = run->commands [run->commands_size - 1];
        job->background = lastCmd->props->bg;
    }

    return job;
}

int job_act_resume (job_t *job)
{
    kill (job->pid, SIGCONT);
    job->state = Running;
    return 0;
}

int job_act_suspend (job_t *job)
{
    kill (job->pid, SIGTSTP);
    job->state = Suspended;
    return 0;
}

int job_find_next_available_id ()
{
    unsigned i;
    for (i = g_job_lowest; i < MAX_JOBS; i++)
    {
        if (!g_jobs [i])
            return i;
    }

    return -1;
}

void job_print_status (job_t *job)
{
    if (!job->background)
        return;

    switch (job->state)
    {
    case Init:
        printf ("[%d] %d\n", job->id, job->pid);
        break;

    case Running:
        printf ("[%d]\tStopped\t'%s'\n", job->id, job->run->line);
        break;

    case Suspended:
        printf ("[%d]\tStopped\t'%s'\n", job->id, job->run->line);
        break;

    case Done:
        printf ("[%d] Done '%s'\n", job->id, job->run->line);
        break;
    }
}

job_t *find_job_by_pid (pid_t pid)
{
    unsigned i;
    for (i = g_job_lowest; i < MAX_JOBS; i++)
    {
        // I know, I know it's gross
        if (g_jobs [i] && g_jobs [i]->pid == pid)
            return g_jobs [i];
    }

    return 0;
}
