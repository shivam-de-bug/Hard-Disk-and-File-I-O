/*Write a program in C to simulate the output redirection using the dup family of
  system calls(dup, dup2, dup3).
  This program consists of 2 parts :
  Part 1: Simple Output Redirection
  - Implement a function
  - That opens a file for writing (From user)
  - Uses dup / dup2 to redirect stdout to file
  - Prints some random text to the redirected stdout
  - Verify that the data is written in the file.
  ./q4 -p1 output_p1.txt // output.txt must contain some random text
  Part 2: Interactive Simulation
  - Simulate a simple shell-like command that redirects the stdout and stderr of any
  internal command into a file. Eg :
  ./q4 -p2 "ls -l" output_p2.txt // output.txt must contain the output of ls -l
  Deliverables :
  - Q4.c containing the implementation for both parts (We should be able to test the
  implementation of both parts)
  - Makefile to compile the program
  - output_p1.txt and output_p2.txt containing the output from both parts.*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

// Part 1: Simple Output Redirection
void redirect_file(const char* filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file for output redirection");
        exit(1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("Error redirecting stdout");
        close(fd);
        exit(1);
    }
    close(fd);
    printf("Redirecting output using dup().\n Output redirection successful.");
    fflush(stdout);
}

// Part 2: Command Output Redirection
void redirect_command(const char* cmd, const char* filename) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) { // Child process
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("Error opening file for command output redirection");
            exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) == -1 || dup2(fd, STDERR_FILENO) == -1) {
            perror("Error redirecting output");
            close(fd);
            exit(1);
        }
        close(fd);

        char* args[4] = { "/bin/sh", "-c", (char*)cmd, NULL };
        execv("/bin/sh", args);
        perror("execv failed");
        exit(1);
    }
    waitpid(pid, NULL, 0);
}

int main(int argc, char* argv[]) {
    argc == 3 && strcmp(argv[1], "-p1") == 0 
        ? redirect_file(argv[2])
    : argc == 4 && strcmp(argv[1], "-p2") == 0
        ? redirect_command(argv[2], argv[3])
    : fprintf(stderr, "Usage:\n  %s -p1 <output_file>\n  %s -p2 \"<command>\" <output_file>\n", argv[0], argv[0]);
    return 0;
}
