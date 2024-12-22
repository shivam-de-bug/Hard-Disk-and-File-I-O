/*Contains a simple implementation of the cp command. We should be
  able to pass two arguments, the first would be a file path, and the second would
  be a directory path. The program will copy the given file to the directory passed
  as the argument. After copying, print out “[file] copied to [dir]” where
  file and dir are the arguments passed to the program. Print an error message
  if insufficient arguments are passed.*/


#include <stdio.h>
#include <stdlib.h>

int check_arguments(int argc) {
    if (argc < 3) {
        fprintf(stderr, " Missing arguments.\nUsage: <source_file> <destination_directory>\n");
        return 0; 
    }
    return 1; 
}


FILE* open_source_file(const char *sourceFilePath) {
    FILE *sourceFile = fopen(sourceFilePath, "rb");
    if (sourceFile == NULL) {
        perror("Error opening source file");
    }
    return sourceFile;
}


int create_destination_path(char *destinationPath, size_t size, const char *destDir, const char *sourceFileName) {
    snprintf(destinationPath, size, "%s/%s", destDir, sourceFileName);
    return 0; 
}


FILE* open_destination_file(const char *destinationPath) {
    FILE *destinationFile = fopen(destinationPath, "wb");
    if (destinationFile == NULL) {
        perror("Error creating destination file");
    }
    return destinationFile;
}


int copy_file_contents(FILE *sourceFile, FILE *destinationFile) {
    int byte;
    while ((byte = fgetc(sourceFile)) != EOF) {
        fputc(byte, destinationFile);
    }
    return 0; 
}


void close_files(FILE *sourceFile, FILE *destinationFile) {
    fclose(sourceFile);
    fclose(destinationFile);
}


int main(int argc, char *argv[]) {
   
    if (!check_arguments(argc)) {
        return 1; 
    }

    
    FILE *sourceFile = open_source_file(argv[1]);
    if (sourceFile == NULL) {
        return 1;
    }

   
    char destinationPath[2048];
    create_destination_path(destinationPath, sizeof(destinationPath), argv[2], argv[1]);

   
    FILE *destinationFile = open_destination_file(destinationPath);
    if (destinationFile == NULL) {
        fclose(sourceFile);
        return 1;
    }

  
    copy_file_contents(sourceFile, destinationFile);

    close_files(sourceFile, destinationFile);
    printf("File '%s' copied to '%s'\n", argv[1], argv[2]);

    return 0;
}
