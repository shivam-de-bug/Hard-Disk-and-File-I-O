/*Contains a simple implementation of the mv command. Should take
  two arguments representing the source and destination path. Should be able to
  move the entire content of the source directory into the destination directory.*/


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

// Function to copy a file from source to destination
int copy_file(const char *src, const char *dest) {
    FILE *src_file = fopen(src, "rb");
    if (src_file == NULL) {
        perror("Error opening source file");
        return -1;
    }

    FILE *dest_file = fopen(dest, "wb");
    if (dest_file == NULL) {
        perror("Error creating destination file");
        fclose(src_file);
        return -1;
    }

    // Copy contents byte by byte
    int byte;
    while ((byte = fgetc(src_file)) != EOF) {
        fputc(byte, dest_file);
    }

    fclose(src_file);
    fclose(dest_file);
    return 0;
}

// Function to create the destination directory if it doesn't exist
int create_dir(const char *path) {
    if (mkdir(path, 0777) == -1 && errno != EEXIST) {
        perror("Error creating directory");
        return -1;
    }
    return 0;
}

// Function to copy contents of a directory
int copy_dir(const char *src, const char *dest) {
    DIR *dir = opendir(src);
    if (dir == NULL) {
        perror("Error opening source directory");
        return -1;
    }

    // Create the destination directory if not exists
    if (create_dir(dest) != 0) {
        closedir(dir);
        return -1;
    }

    struct dirent *entry;
    char source_file_path[1024], destination_file_path[1024];

    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(source_file_path, sizeof(source_file_path), "%s/%s", src, entry->d_name);
        snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s", dest, entry->d_name);

        struct stat info;
        if (stat(source_file_path, &info) == -1) {
            perror("Error reading file status");
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            // Recursively copy subdirectories
            if (copy_dir(source_file_path, destination_file_path) != 0) {
                perror("Error copying directory");
            }
        } else if (S_ISREG(info.st_mode)) {
            // Copy files
            if (copy_file(source_file_path, destination_file_path) != 0) {
                perror("Error copying file");
            }
        }
    }

    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {
    // Check for  arguments
    if (argc < 3) {
        fprintf(stderr, "Missing arguments. Usage: %s <source_dir> <destination_dir>\n", argv[0]);
        return 1;
    }

    // Copy the contents of the source directory to the destination
    if (copy_dir(argv[1], argv[2]) == 0) {
        printf("Contents of '%s' moved to '%s'\n", argv[1], argv[2]);
    } else {
        fprintf(stderr, "Failed to move directory contents\n");
        return 1;
    }

    return 0;
}
