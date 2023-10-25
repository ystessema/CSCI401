#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

pid_t foreground_pid = -1;

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;

void print_prompt() {
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s> ", cwd);
  } else {
    perror("getcwd() error");
    exit(1);
  }
}

void handle_sigint(int sig) {
  (void)sig; // suppress unused variable warning
  printf("\n");
  print_prompt();
  fflush(stdout);
}

void handle_sigalrm(int sig) {
  (void)sig; // suppress unused variable warning

  if (foreground_pid > 0) {
    kill(foreground_pid, SIGKILL);
    printf("\nForeground process %d terminated due to timeout.\n", foreground_pid);
    foreground_pid = -1; 
    print_prompt();
    fflush(stdout);
  }
}


int main() {
    // Stores the string typed into the command line.
    signal(SIGINT, handle_sigint);
    signal(SIGALRM, handle_sigalrm);
    char command_line[MAX_COMMAND_LINE_LEN];
    while (true) {
      print_prompt();
      fflush(stdout);

      if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
        fprintf(stderr, "fgets error");
        exit(0);
      }

      if (feof(stdin)) {
        printf("\n");
        fflush(stdout);
        fflush(stderr);
        return 0;
      }
      char *token = strtok(command_line, delimiters);
      char *arguments[MAX_COMMAND_LINE_ARGS];
      int arg_count = 0;

      while (token != NULL) {
        arguments[arg_count++] = token;
        token = strtok(NULL, delimiters);
      }
      arguments[arg_count] = NULL;
      
      bool background = false;
      if (arg_count > 0 && strcmp(arguments[arg_count - 1], "&") == 0) {
        background = true;
        arguments[--arg_count] = NULL;  // remove & from arguments
      }

        // Check for built-in commands
      if (strcmp(arguments[0], "cd") == 0) {
        if (chdir(arguments[1]) != 0) {
          perror("cd failed");
        }
          continue;
      }
      if (strcmp(arguments[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
          printf("%s\n", cwd);
        } else {
          perror("pwd failed");
        }
        continue;
     }

      int i;
      if (strcmp(arguments[0], "echo") == 0) {
        for (i = 1; i < arg_count; i++) {
          if (arguments[i][0] == '$') {
            char *value = getenv(arguments[i] + 1);
            if (value) {
              printf("%s ", value);
            }
          } 
          else {
            printf("%s ", arguments[i]);
          }
        }
        printf("\n");
        continue;
      }
      if (strcmp(arguments[0], "exit") == 0) {
        exit(0);
      }

      if (strcmp(arguments[0], "env") == 0) {
        char **env;
        for (env = environ; *env; env++) {
          printf("%s\n", *env);
        }
        continue;
      }

      
      if (strcmp(arguments[0], "setenv") == 0) {
        if (arg_count < 3) {
            fprintf(stderr, "setenv: Too few arguments\n");
        } else {
            setenv(arguments[1], arguments[2], 1);
        }
        continue;
    }

      // Handle external commands
      pid_t pid = fork();

      if (pid == -1) {
          perror("fork failed");
          continue;
      }
      if (background) {
        printf("Started process %d\n", pid);
      } 
      // This block will get executed by the child process
      if (pid == 0) {
        execvp(arguments[0], arguments);
        fprintf(stderr, "execvp() failed: %s\n", strerror(errno));
        exit(1);
      }
      
      if (!background) {
        foreground_pid = pid; // save the PID of the foreground process
        alarm(10); // set the alarm for 10 seconds

        int status;
        waitpid(pid, &status, 0);

        alarm(0); // cancel the alarm
        foreground_pid = -1; // reset the PID after process completion
    }
    //   if (!background) {
    // // Parent process waits for the child to finish
    //     int status;
    //     waitpid(pid, &status, 0);
    //   }
  

  return -1;
}    

        // TODO:
        // 
        
			  // 0. Modify the prompt to print the current working directory
			  
			
        // 1. Tokenize the command line input (split it on whitespace)

      
        // 2. Implement Built-In Commands
      
    
        // 3. Create a child process which will execute the command line input

  
        // 4. The parent process should wait for the child to complete unless its a background process
      
      
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    
    // This should never be reached.