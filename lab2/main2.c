#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_COUNT 30

void ChildProcess(int childNumber) {
    for (int i = 1; i <= MAX_COUNT; i++) {
        printf("Child %d is running, value = %d\n", childNumber, i);
    }
    printf("Child %d is done.\n", childNumber);
}

void ParentProcess(void) {
    int status;
    pid_t child1, child2;

    child1 = fork();
    if (child1 == 0) {
        ChildProcess(1);
        _exit(0); // Terminate the child process
    } else if (child1 < 0) {
        perror("Fork failed for child 1");
    }

    child2 = fork();
    if (child2 == 0) {
        ChildProcess(2);
        _exit(0); // Terminate the child process
    } else if (child2 < 0) {
        perror("Fork failed for child 2");
    }

    // Parent process waits for both child processes to complete
    waitpid(child1, &status, 0);
    waitpid(child2, &status, 0);

    printf("Parent is done.\n");
}

int main(void) {
    ParentProcess();
    return 0;
}