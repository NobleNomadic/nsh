// main.c - Main shell program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

const char *shellPrompt = "# ";

// Spawn process from filename and wait for it to exit before returning
void spawnProcess(char *filename) {
  // Keep track of child status
  int returnStatus;

  // Fork process
  int childPid = fork();
  if (childPid == 0) {
    // Child code
    execvp(filename, 0); // Execute the inputted filename
    exit(0);
  }

  // Parent process code
  else {
    waitpid(childPid, &returnStatus, 0);
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
