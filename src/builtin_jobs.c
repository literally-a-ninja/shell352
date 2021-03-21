#include "buongiorno/common.h"
#include "jobs.h"
#include "shell352.h"
#include <stdio.h>
#include <stdlib.h>

int builtin_fg (cmd_t *cmd, struct environment *e)
{
    fprintf (stdout, "fg: There are no suitable jobs.\n");
    return 0;
}

int builtin_bg (cmd_t *cmd, struct environment *e)
{
    fprintf (stdout, "bg: There are no suitable jobs.\n");
    return 0;
}

int builtin_jobs (cmd_t *cmd, struct environment *e)
{
    if (!g_job_count)
    {
        fprintf (stdout, "jobs: There are no suitable jobs.\n");
        return 0;
    }

    fprintf (stdout, "Jobs\tGroup\tState\tCommand\n");
    unsigned i;
    job_t *job;
    for (i = g_job_lowest; i < MAX_JOBS; i++)
    {
        // I know, I know it's gross
        if ((job = g_jobs [i]))
        {
            fprintf (stdout, "%d\t%d\t%s\t%s\n", job->id, job->pid,
                     job_state_name [job->state], job->run->line);
        }
    }
    return 0;
}
