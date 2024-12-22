/*This program will run all the executables of the above programs as child
  processes using the fork-wait-exec sequence. First, create 3 child processes
  using fork. Then inside the child processes, call one of the exec functions to
  execute the above programs. One child will run one program. The parent process
  will wait for the child processes to finish and then return.
  ○ The ‘list’ program should list the files in the parent of the current folder.
  ○ The ‘countw’ program should count the words in the ‘Makefile’.
  ○ The ‘copy’ program should copy the ‘list.c’ inside this folder to the parent
  of this folder.
  ○ The ‘move’ program should be able to move the entire contents of the q5
  folder into a new directory called q5_files.*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid1, pid2, pid3, pid4;
    
    // Fork for the first child (list)
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork failed for list");
        exit(1);
    }
    if (pid1 == 0) {
        execl("./list", "list", "..", NULL);
        perror("execl failed for list");
        exit(1);
    }
    
    // Fork for the second child (countw)
    pid2 = fork();
    if (pid2 == -1) {
        perror("fork failed for countw");
        exit(1);
    }
    if (pid2 == 0) {
        execl("./countw", "countw", "Makefile", NULL);
        perror("execl failed for countw");
        exit(1);
    }
    
    // Fork for the third child (copy)
    pid3 = fork();
    if (pid3 == -1) {
        perror("fork failed for copy");
        exit(1);
    }
    if (pid3 == 0) {
        execl("./copy", "copy", "list.c", "..", NULL);
        perror("execl failed for copy");
        exit(1);
    }
    
    // Fork for the fourth child (move)
    pid4 = fork();
    if (pid4 == -1) {
        perror("fork failed for move");
        exit(1);
    }
    if (pid4 == 0) {
        execl("./move", "move", ".", "../q5_files", NULL);
        perror("execl failed for move");
        exit(1);
    }
    
    // Wait for all children to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    waitpid(pid4, NULL, 0);
    
    return 0;
}
