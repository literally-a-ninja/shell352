#ifndef H_JOBS
#define H_JOBS
#define MAX_JOBS 15

#include "buongiorno/grammar.h"

typedef enum job_state
{
    Init,
    Running,
    Suspended
} job_state;

typedef struct job_t
{
    cmd_t *cmd;
    pid_t pid;
    job_state state;
} job_t;

extern job_t g_jobs [MAX_JOBS];
extern unsigned g_job_count;
extern job_t g_job_last;

job_t *ctor_job_t (cmd_t *cmd, pid_t pid);

void job_set_state (job_t *job, job_state state);
#endif
