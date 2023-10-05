/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int flag = 0;

void handler(int signum)
{ //signal handler
  printf("Hello World!\n");
  flag = 1;
  alarm(1);
  //exit(1); //exit after printing
}

int main(int argc, char * argv[])
{
  signal(SIGALRM,handler); //register handler to handle SIGALRM
  alarm(1); //Schedule a SIGALRM for 1 second
  while(1){
    if (flag)
    {
      printf("Turing was right!\n");
      flag = 0; // Reset the flag
    }
    sleep(1);
  }
 //busy wait for signal to be delivered
  return 0; //never reached
}