#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void ParentProcess(int []);
void ChildProcess(int []);

int main(int argc, char *argv[])
{
    int ShmID;
    int *ShmPTR;
    pid_t pid;
    int status;

    if (argc != 5)
    {
        printf("Use: %s #1 #2 #3 #4\n", argv[0]);
        exit(1);
    }

    ShmID = shmget(IPC_PRIVATE, 4 * sizeof(int), IPC_CREAT | 0666);
    if (ShmID < 0)
    {
        printf("*** shmget error (server) ***\n");
        exit(1);
    }
    printf("Server has received a shared memory of four integers...\n");

    ShmPTR = (int *)shmat(ShmID, NULL, 0);
    if (*ShmPTR == -1)
    {
        printf("*** shmat error (server) ***\n");
        exit(1);
    }
    printf("Server has attached the shared memory...\n");

    ShmPTR[0] = atoi(argv[1]);
    ShmPTR[1] = atoi(argv[2]);
    ShmPTR[2] = atoi(argv[3]);
    ShmPTR[3] = atoi(argv[4]);
    printf("Server has filled %d %d %d %d in shared memory...\n",
           ShmPTR[0], ShmPTR[1], ShmPTR[2], ShmPTR[3]);

    printf("Server is about to fork a child process...\n");
    pid = fork();
    if (pid < 0)
    {
        printf("*** fork error (server) ***\n");
        exit(1);
    }
    else if (pid == 0)
    {
        ChildProcess(ShmPTR);
        exit(0);
    }

    ParentProcess(ShmPTR);

    wait(&status);
    printf("Server has detected the completion of its child...\n");
    shmdt((void *)ShmPTR);
    printf("Server has detached its shared memory...\n");
    shmctl(ShmID, IPC_RMID, NULL);
    printf("Server has removed its shared memory...\n");
    printf("Server exits...\n");
    exit(0);
}

void ParentProcess(int SharedMem[])
{
    printf("Dear old Dad process started\n");

    srand(time(NULL)); // Seed for random number generation

    for (int i = 0; i < 25; ++i)
    {
        sleep(rand() % 6); // Sleep for a random amount of time between 0-5 seconds

        int account = SharedMem[0];

        while (SharedMem[3] != 0)
        {
            // Do nothing while it's not Dad's turn
        }

        if (account <= 100)
        {
            int balance = rand() % 101;

            if (balance % 2 == 0)
            {
                SharedMem[0] += balance;
                printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, SharedMem[0]);
            }
            else
            {
                printf("Dear old Dad: Doesn't have any money to give\n");
            }

            SharedMem[3] = 1; // Set turn to Child's turn
        }
        else
        {
            printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
        }
    }
}

void ChildProcess(int SharedMem[])
{
    printf("Poor Student process started\n");

    srand(time(NULL)); // Seed for random number generation

    for (int i = 0; i < 25; ++i)
    {
        sleep(rand() % 6); // Sleep for a random amount of time between 0-5 seconds

        int account = SharedMem[0];

        while (SharedMem[3] != 1)
        {
            // Do nothing while it's not Student's turn
        }

        int balance = rand() % 51;

        printf("Poor Student needs $%d\n", balance);

        if (balance <= account)
        {
            SharedMem[0] -= balance;
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, SharedMem[0]);
        }
        else
        {
            printf("Poor Student: Not Enough Cash ($%d)\n", account);
        }

        SharedMem[3] = 0; // Set turn to Dad's turn
    }
}
