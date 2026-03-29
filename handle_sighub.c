#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdatomic.h>

volatile atomic_int verbose = 0;
volatile atomic_int running = 1;
volatile atomic_int counter = 0;

/* SIGINT: stop program */
void handle_sigint(int sig)
{
    printf("\n[INTERRUPT] SIGINT received - stopping program\n");
    running = 0;
}

/* SIGUSR1: reset counter */
void handle_sigusr1(int sig)
{
    printf("\n[INTERRUPT] SIGUSR1 received - resetting counter\n");
    counter = 0;
}

/* SIGUSR2: report counter */
void handle_sigusr2(int sig)
{
    printf("\n[STATS] Current counter value: %d\n", counter);
}

void handle_sigterm(int sig)
{
    printf("\n[SIGTERM] Termination signal received. Cleaning up...\n");
    running = 0;
}


void handle_sighup(int sig)
{
    verbose ^= 1;  // toggle 0 ↔ 1

    printf("\n[SIGHUP] Verbose mode: %s\n",
           verbose ? "ON" : "OFF");
}

int main()
{
    signal(SIGINT, handle_sigint);
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);
    signal(SIGTERM, handle_sigterm);
    signal(SIGHUP, handle_sighup);

    printf("Interrupt demo running. PID: %d\n", getpid());
    printf("Ctrl+C -> stop program\n");
    printf("kill -USR1 %d -> reset counter\n", getpid());
    printf("kill -USR2 %d -> show counter value\n\n", getpid());

    while (running)
    {
        counter++;
       if (verbose)
            {
            printf("Iteration: %d | PID: %d\n", counter, getpid());
        }
        else
            {
            printf("%d\r", counter);
            }

fflush(stdout);
        fflush(stdout);
        sleep(1);
    }

    printf("\nProgram exited after %d iterations.\n", counter);

    return 0;
}
