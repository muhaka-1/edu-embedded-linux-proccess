#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void start_child() {
    pid_t pid = fork();

    if (pid == 0) { // Child Process
        printf("[CHILD] Started (PID: %d). Working hard...\n", getpid());
        sleep(5); 
        printf("[CHILD] Oh no, I crashed!\n");
        exit(1); // Simulate a crash
    } 
    else if (pid > 0) { // Parent Process
        printf("[WATCHDOG] Monitoring Child (PID: %d)\n", pid);
        int status;
        waitpid(pid, &status, 0); // Wait for the specific child

        if (WIFEXITED(status)) {
            printf("[WATCHDOG] Child exited with code %d. Restarting...\n", WEXITSTATUS(status));
            sleep(2);
            start_child(); // RESTART the child!
        }
    }
}

void handle_shutdown(int sig) {
    printf("\n[WATCHDOG] Shutdown signal received. Killing all processes...\n");
    kill(0, SIGKILL); // Kill the entire process group
    _exit(0);
}

int main() {
    // Register professional signal handler for the Watchdog itself
    struct sigaction sa;
    sa.sa_handler = handle_shutdown;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    printf("Watchdog System Active (PID: %d). Press Ctrl+C to stop everything.\n", getpid());
    
    start_child();

    return 0;
}
