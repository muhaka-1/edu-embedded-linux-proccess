#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdatomic.h>

volatile atomic_int running = 1;
volatile atomic_int counter = 0;

/* Ctrl+C */
void handle_sigint(int sig) {
    printf("\n[INTERRUPT] SIGINT received\n");
    running = 0;
}

/* SIGTERM (you WON'T see this while blocked) */
void handle_sigterm(int sig) {
    printf("\n[INTERRUPT] SIGTERM received\n");
    running = 0;
}

int main() {

    /* Register handlers */
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigterm);

    /* BLOCK SIGTERM */
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTERM);

    sigprocmask(SIG_BLOCK, &mask, NULL);

    printf("SIGTERM is now BLOCKED!\n");
    printf("PID: %d\n", getpid());
    printf("Try:\n");
    printf("  kill %d        (won't work)\n", getpid());
    printf("  kill -KILL %d  (will work)\n\n", getpid());

    while (running) {
        counter++;
        // printf("Loop: %d\r", counter);
        if (counter == 10) {
            printf("\n[INFO] Unblocking SIGTERM now!\n");
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
        }
        fflush(stdout);
        sleep(1);
    }

    printf("\nProgram exited cleanly\n");
    return 0;
}
