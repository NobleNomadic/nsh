// main.c - Main shell program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

const char *shellPrompt = "# ";

// Spawn process from filename and arguments, and wait for it to exit before returning
void spawnProcess(char *input) {
  // Split the input into command and arguments
  char *args[128];  // Array to hold the command and arguments
  char *token = strtok(input, " ");  // Tokenize the input by spaces
  int i = 0;

  // Populate the args array
  while (token != NULL) {
    args[i] = token;
    token = strtok(NULL, " ");
    i++;
  }

  args[i] = NULL;  // Null-terminate the array (execvp needs this)

  // Fork process
  int childPid = fork();
  if (childPid == 0) {
    // Child code
    execvp(args[0], args); // Execute the command with arguments
    exit(1);  // Exit if execvp fails
  }

  // Parent process code
  else {
    int returnStatus;
    waitpid(childPid, &returnStatus, 0);  // Wait for the child process to exit
    return;
  }
}

// Interactive loop function
void shellLoop() {
  while (1) {
    // Print prompt
    printf(shellPrompt);

    // Get input
    char inputBuffer[128];
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    strtok(inputBuffer, "\n");

    // Check if 'exit' was entered
    if (strcmp(inputBuffer, "exit") == 0) {
      exit(0);
    }

    // Check if 'cd' was entered
    if (strncmp(inputBuffer, "cd", 2) == 0) {
      // Split input into command and arguments
      char *path = strtok(inputBuffer + 2, " ");  // Get the path after "cd"

      // Default to home directory if no argument is given
      if (path == NULL) {
        path = getenv("HOME");
      }

      // Change directory
      if (chdir(path) == -1) {
        // If chdir fails, print an error message
        perror("cd failed");
      }
    }

    // Spawn process and wait for it to exit, then continue
    spawnProcess(inputBuffer);
  }
  return;
}

// ==== ENTRY FUNCTION ====
int main() {
  shellLoop();
  return 0;
}
