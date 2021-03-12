#include "jobs.h"
#include <stdlib.h>

job_t *ctor_job_t (cmd_t *cmd, pid_t pid)
{
    job_t *job = malloc (sizeof (job_t));
    job->cmd   = cmd;
    job->pid   = pid;

    return job;
}
