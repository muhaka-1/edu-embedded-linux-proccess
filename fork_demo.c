#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    printf("Master Process started. PID: %d\n", getpid());

    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        // --- THIS CODE RUNS IN THE CHILD ONLY ---
        printf("[CHILD] I am alive! PID: %d, My Parent is: %d\n", getpid(), getppid());
        int count = 0;
        while(1) {
            printf("[CHILD] Working... iteration %d\n", ++count);
            sleep(2);
        }
    } else {
        // --- THIS CODE RUNS IN THE PARENT ONLY ---
        printf("[PARENT] Child created with PID: %d\n", child_pid);
        printf("[PARENT] I am now waiting for my child to exit...\n");

        int status;
        // waitpid pauses the parent until the child finishes
        waitpid(child_pid, &status, 0); 

        printf("[PARENT] Child has terminated. I am now exiting too.\n");
    }

    return 0;
}
