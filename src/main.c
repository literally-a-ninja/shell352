#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// buongiornolib gets loaded first!
#include "buongiorno/common.h"
#include "buongiorno/grammar.h"
#include "buongiorno/parse.h"
#include "buongiorno/path.h"
#include "buongiorno/string.h"

#include "exec.c"
#include "jobs.c"
#include "jobs.h"
#include "shell352.h"

void recieve (int iSignal)
{
    switch (iSignal)
    {
    /* Foward SIGTSTP to the currently running forground process. */
    case SIGTSTP:
    case SIGTERM:
    case SIGQUIT:
        if (!g_fgJob)
            break;
        kill (g_fgJob->pid, iSignal);
        g_fgJob->background = 1;
        break;

    /* Double print new line to let the user know we're done. */
    case SIGINT:
        printf ("\n");
        g_shellStatus |= SHELL_DIE;
        break;

    default:
        return;
        break;
    }

    printf ("\n");
    signal (iSignal, &recieve);
}

void init ()
{
    signal (SIGINT, &recieve);
    signal (SIGTSTP, &recieve);
    signal (SIGQUIT, &recieve);
    signal (SIGTERM, &recieve);

    g_env          = malloc (sizeof (struct environment));
    g_env->m_ptrWd = malloc (sizeof (char) * PATH_MAX_DIR_LENGTH);
    getcwd (g_env->m_ptrWd, PATH_MAX_DIR_LENGTH);
    // g_env->m_ptrWd = realloc(&g_env->m_ptrWd, sizeof(char) *
    // B_strlen(g_env->m_ptrWd));

    g_fgJob      = 0;
    g_job_count  = 1;
    g_job_lowest = 1;
    g_run        = malloc (sizeof (run_t));
}

void shutdown ()
{
    free (g_run);
    free (g_env->m_ptrWd);
    free (g_env);

    pthread_join (g_inputThread, NULL);

#ifdef DEBUG
    printf (
        "D: shutdown() : Successfully freed all resources, returning to main() "
        "for termination.\n");
#endif
}

void check_jobs ()
{
    if (!g_job_count)
        return;

    int status, pid;
    while ((pid = waitpid (-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0)
    {
        job_t *job;
        if (!(job = find_job_by_pid (pid)))
        {
            continue;
        }

        if (job->state == Done || WIFEXITED (status))
        {
            unsigned id = job->id;

            if (job->background)
            {
                job->state = Done;
                job_print_status (job);
            }

            g_job_count--;
            g_job_lowest = min (g_job_lowest, id);
            g_jobs [id]  = 0;

            free (job);
            return;
        }

        if (WIFSTOPPED (status))
        {
            job->state = Suspended;
        }
        else if (WIFCONTINUED (status))
        {
            job->state = Running;
        }

        job_print_status (job);
    }
}

void *main_input (void *vargp)
{
    char input [80];
    while (!feof (stdin))
    {
        check_jobs ();
        printf ("352 %s# ", g_env->m_ptrWd);
        fflush (stdout);
        fgets (input, 80, stdin);

        array_t *a = B_parse_input_runs (input, 80);

        // Reject blank lines
        if (!a->length)
            continue;

        unsigned i;
        for (i = 0; i < a->length; i++)
        {
            pid_t pid;
            run_t *run = a->array [i];

            if (!run->commands_size)
                continue;

            job_t *job = ctor_job_t (run);

            job->id          = job_find_next_available_id ();
            g_jobs [job->id] = job;
            g_fgJob          = job;
            g_job_count++;

            exec_job (job);

            if (job->background)
            {
                job_print_status (job);
                continue;
            }

            int status;
            waitpid (-1, &status, WNOHANG);
            if (WIFEXITED (status))
            {
                close (job->pipe [IWRITE]);
                char c;
                while ((read (job->pipe [IREAD], &c, sizeof (char))) > 0)
                    write (STDOUT_FILENO, &c, sizeof (char));

                close (job->pipe [IREAD]);
            }
        }
    }

    pthread_exit (0);
}

int main (int argc, char *argv [], char *envp [])
{
    init ();

    pthread_t g_inputThread;
    pthread_create (&g_inputThread, NULL, &main_input, envp);

    while (1)
    {
        // User closed stdin (typically means we should exit)
        if (g_shellStatus & SHELL_DIE)
        {
#ifdef DEBUG
            printf ("D: SHELL_DIE caught\n");
#endif
            break;
        }
    }

    shutdown ();

    return 0;
}
