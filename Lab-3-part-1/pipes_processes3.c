#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <search_string>\n", argv[0]);
        exit(1);
    }

    int pipefd1[2], pipefd2[2];
    int pid1, pid2;

    char *cat_args[] = {"cat", "scores", NULL};
    char *grep_args[] = {"grep", argv[1], NULL};
    char *sort_args[] = {"sort", NULL};

    // Create two pipes
    pipe(pipefd1);
    pipe(pipefd2);

    // First fork (to run cat)
    pid1 = fork();

    if (pid1 == 0) {
        // First child (cat)

        // Replace standard output with output part of pipefd1
        dup2(pipefd1[1], 1);

        // Close unused pipes
        close(pipefd1[0]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        // Execute cat
        execvp("cat", cat_args);
        perror("execvp of cat failed");
        exit(1);
    }

    // Second fork (to run grep)
    pid2 = fork();

    if (pid2 == 0) {
        // Second child (grep)

        // Replace standard input with input part of pipefd1
        dup2(pipefd1[0], 0);

        // Replace standard output with output part of pipefd2
        dup2(pipefd2[1], 1);

        // Close unused pipes
        close(pipefd1[1]);
        close(pipefd2[0]);

        // Execute grep
        execvp("grep", grep_args);
        perror("execvp of grep failed");
        exit(1);
    }

    // Parent process (sort)

    // Replace standard input with input part of pipefd2
    dup2(pipefd2[0], 0);

    // Close unused pipes
    close(pipefd1[0]);
    close(pipefd1[1]);
    close(pipefd2[1]);

    // Execute sort
    execvp("sort", sort_args);
    perror("execvp of sort failed");
    exit(1);

    return 0;
}
