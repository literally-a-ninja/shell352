#ifndef H_JOBS
#define H_JOBS
#define MAX_JOBS (1 << 5) - 1

#include "buongiorno/grammar.h"

typedef enum job_state
{
    Init      = 0,
    Running   = 1,
    Suspended = 2,
    Done      = 3,
} job_state;

const char *const job_state_name [] = {
    [Init]      = "",
    [Running]   = "running",
    [Suspended] = "stopped",
    [Done]      = "done",
};

typedef struct job_t
{
    unsigned id;
    run_t *run;
    pid_t pid;
    job_state state;
    int pipe [2];
    int background;
} job_t;

job_t *g_jobs [MAX_JOBS];
unsigned g_job_count;
unsigned g_job_lowest;

/**
 * Constructs an allocated job.
 **/
job_t *ctor_job_t (run_t *run);

/**
 * Tells the program to resume
 **/
int job_act_resume (job_t *job);

/**
 * Tells the program to suspend
 **/
int job_act_suspend (job_t *job);

/**
 * Tells the program to suspend
 **/
int job_find_next_available_id ();

/**
 * Prints the state of the job
 **/
void job_print_status (job_t *job);

/**
 * Prints the state of the job
 **/
job_t *find_job_by_pid (pid_t pid);
#endif
