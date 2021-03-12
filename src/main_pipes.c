#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main (int argc, char *argv [], char *envp [])
{
    char c [1];
    int o [2], pid, nbytes;

    if (pipe (o) < 0)
        exit (1);

    /* continued */

    switch (pid = fork ())
    {
    case 0:
        // stdout -> pipe o
        // dup2 (o [WRITE], STDOUT_FILENO);
        // close (o [WRITE]);
        // close (o [READ]);

        // int i2 [2], o2 [2];
        // if (pipe (i2) < 0 || pipe (o2) < 0)
        //     exit (1);

        // // Child code
        // if (!fork ())
        // {
        //     // stdin  <- pipe i2
        //     // stdout -> pipe o2
        //     dup2 (i2 [READ], STDIN_FILENO);
        //     dup2 (o2 [WRITE], STDOUT_FILENO);
        //     char *const args [] = {"ls", NULL};
        //     execvp ("ls", args);
        //     exit (0);
        // }

        // close (STDIN_FILENO);
        // dup2 (o2 [READ], STDIN_FILENO);

        // // wait for inner child to exit.
        // wait (NULL);
        // close (o2 [WRITE]);

        // // execute secondary command
        // char *const args [] = {"grep", "Make", NULL};
        // execvp ("grep", args);

        write (o [1], "1. Child waves\n", 16);
        close (o [1]);
        exit (0);
        break;

    default:
        wait (NULL);
        close (o [1]);

        char c;
        unsigned nbytes;
        while ((nbytes = read (o [0], &c, sizeof (char)) > 0))
        {
            write (STDOUT_FILENO, &c, sizeof (char));
        }
        printf ("2. Parent waves back\n");

        // wait for child to exit
        // wait (NULL);
        // close (o [WRITE]);

        // // read what's left behind.
        // while ((nbytes = read (o [READ], &c, sizeof (char))) > 0)
        // {
        //     write (STDOUT_FILENO, &c, sizeof (char));
        // }
        // close (o [READ]);
        break;

    case -1:
        fprintf (stderr, "E: Failed to fork process\n");
        break;
    }

    return 0;
}
