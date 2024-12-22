/*Contains a simple implementation of the wc command. It will print out
  the word count of the file passed to it as an argument. If no argument is passed,
  it should throw an error message. Consider words as simply whitespace
  separated entities.*/


#include <stdio.h>
#include <ctype.h>

int no_of_words(FILE *file) {
    int words = 0;
    int within_word = 0;
    int ch;
    
    while ((ch = fgetc(file)) != EOF) {
       // Set `within_word` to 0 if the character is whitespace, or increment the word count and set `within_word` to 1 if a new word starts.
    within_word = isspace(ch) ? 0 : (!within_word ? (words++, 1) : within_word);

    }
    
    return words;
}

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        fprintf(stderr, "Error: No file specified\n");
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Count words
    int words = no_of_words(file);
    printf("Word count: %d\n", words);
    fclose(file);
    
    return 0;
}