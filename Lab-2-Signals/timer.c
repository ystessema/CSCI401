#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int alarm_count = 0;
time_t start_time;

void alarm_handler(int signum)
{
    alarm_count++;
    alarm(1); // Schedule another SIGALRM for 1 second
}

void sigint_handler(int signum)
{
    time_t end_time = time(NULL);
    double execution_time = difftime(end_time, start_time);

    printf("\nProgram executed for %d seconds and received %d alarms.\n", (int)execution_time, alarm_count);
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, alarm_handler); // Register handler for SIGALRM
    signal(SIGINT, sigint_handler); // Register handler for SIGINT (CTRL-C)

    start_time = time(NULL); // Record the start time

    printf("Running... Press CTRL-C to exit.\n");

    alarm(1); // Schedule the initial SIGALRM for 1 second

    while (1)
    {
        // Keep the program running
        sleep(1);
    }

    return 0; // Never reached
}
