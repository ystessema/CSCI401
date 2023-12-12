#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

#include "process.h"
#include "util.h"

#define DEBUG 0         // Change this to 1 to enable verbose output

typedef int (*Comparer) (const void *a, const void *b);

int my_comparer(const void *this, const void *that) {
    // Cast the void pointers to Process pointers
    const Process *process1 = (const Process *)this;
    const Process *process2 = (const Process *)that;

    // First, compare based on priority
    if (process1->priority > process2->priority) {
        return -1;
    } else if (process1->priority < process2->priority) {
        return 1;
    } else {
        // If the priority is the same, then sort based on arrival time
        if (process1->arrival_time < process2->arrival_time) {
            return -1;
        } else if (process1->arrival_time > process2->arrival_time) {
            return 1;
        } else {
            return 0;
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: ./func-ptr <input-file-path>\n");
        fflush(stdout);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        fprintf(stderr, "Error: Invalid filepath\n");
        fflush(stdout);
        return 1;
    }

    Process *processes = parse_file(input_file);

    Comparer process_comparer = &my_comparer;

#if DEBUG
    for (int i = 0; i < P_SIZE; i++) {
        printf("%d (%d, %d) ", processes[i].pid, processes[i].priority, processes[i].arrival_time);
    }
    printf("\n");
#endif

    qsort(processes, P_SIZE, sizeof(Process), process_comparer);

    for (int i = 0; i < P_SIZE; i++) {
        printf("%d (%d, %d)\n", processes[i].pid, processes[i].priority, processes[i].arrival_time);
    }
    fflush(stdout);
    fflush(stderr);

    free(processes);
    fclose(input_file);
    return 0;
}
