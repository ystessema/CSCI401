#include  <stdio.h>
#include  <string.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define   MAX_COUNT  200
#define   BUF_SIZE   100

void ChildProcess(int childNumber){
     int iterations = (rand() % MAX_COUNT) + 1;
     pid_t pid = getpid();
     pid_t ppid = getppid();

     for (int i = 1; i <= iterations; i++) {
        printf("Child %d (PID: %d) is going to sleep!\n", childNumber, pid);
        int sleepTime = (rand() % BUF_SIZE) + 1;
        sleep(sleepTime);
        printf("Child %d (PID: %d) is awake!\nWhere is my Parent: %d?\n", childNumber, pid, ppid);
    }

    printf("Child %d (PID: %d) is done!\n", childNumber, pid);
    exit(0);
}

int main(void) {
    srand(time(NULL)); 

    pid_t child1, child2;
    
    child1 = fork();
    if (child1 == 0) {
        ChildProcess(1);
    } else if (child1 < 0) {
        perror("Fork failed for child 1");
    }

    child2 = fork();
    if (child2 == 0) {
        ChildProcess(2);
    } else if (child2 < 0) {
        perror("Fork failed for child 2");
    }

    int status;
    waitpid(child1, &status, 0);
    waitpid(child2, &status, 0);

    printf("Parent (PID: %d) has completed.\n", getpid());

    return 0;
}