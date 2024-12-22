/*Contains a simple implementation of the ls command. When a directory
  path is passed to this program as an argument, it will print out all the
  subdirectories and files inside that directory. If no argument is passed, it lists the
  current directory. Use the directory related C functions.*/


#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *item;
    
    const char *path;

    //  directory path
    if (argc > 1) {
        path = argv[1]; 
    } else {
        path = "."; // current directory
    }
    
    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }
    
    //  print directory contents
    printf("Contents of directory %s:\n", path);
    while ((item = readdir(dir)) != NULL) {
      
        if (item->d_name[0] != '.') {
            printf("%s\n", item->d_name);
        }
    }
    
    
    closedir(dir);
    
    return 0;
}