/*Write a program in C to retrieve the temperature measurement values from a text
  file and calculate the min, mean and max temperature per weather station. You need to
  implement two variations of this program (1) using mmap and (2) using fread (or any
  file read function).
  Input : Text file containing 10 million rows (as a demo we take 75 rows now )of weather station data where each line
  contains the data in the following format:

  station_name;temperature

  Eg :
  Delhi;30.9
  Lalmonirhat;23.0
  Fernley;39.8
  Delhi;29.6
  San Antonio de los Baños;24.9
  Andalusia;60.5
  Shuilin;16.2
  Output : The program should output the following for each weather station
  Delhi min=<min> mean=<mean> max=<max>
  Fernley min=<min> mean=<mean> max=<max>
  Andalusia min=<min> mean=<mean> max=<max>

  Along with this the program must also print the total time taken to process the dataset
  using fread and mmap.
  Notes:
  - You are allowed to use system calls like madvice with MADV_SEQUENTIAL
  in order to improve the performance of mmap.
  Deliverables:
  - Q1.c contains the implementation of this program using fread and mmap.
  - Makefile to compile the program.
  - A small writeup containing why one approach is better/more efficient than the
  other approach for this dataset.
  - You can also include the number of system calls invoked using strace to
  indicate the performance bottlenecks.
  - You can also explain how each of these API’s work under the hood in
  order to explain the performance bottlenecks.

  - Output containing the min, mean and max temperature value for each weather
  station along with total time required for each approach to process the dataset.*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

// Define a struct to store temperature data for each station
typedef struct {
    double min;
    double max;
    double sum;
    int count;
} StationStats;

// Function to update weather statistics
void update_stats(StationStats *stats, double temperature) {
    if (temperature < stats->min) stats->min = temperature;
    if (temperature > stats->max) stats->max = temperature;
    stats->sum += temperature;
    stats->count++;
}

// Helper function to process a line
void process_line(char *line, StationStats *stations, int *station_count, char **station_names,int max_stations) {
    //getting station name
    char *station_name = strtok(line, ";");
    //getting temperature reading
    char *temp_str = strtok(NULL, "\n");
    if (!station_name || !temp_str) return;

    //convert string to double
    double temperature = atof(temp_str);

    for (int i = 0; i < *station_count; i++) {
        if (strcmp(station_names[i], station_name) == 0) {   //if station has already appeared
            update_stats(&stations[i], temperature);
            return;
        }
    }

    //making a new entry if a station has not already appeared before
    if (*station_count < max_stations) {
        station_names[*station_count] = strdup(station_name);
        stations[*station_count].min = temperature;
        stations[*station_count].max = temperature;
        stations[*station_count].sum = temperature;
        stations[*station_count].count = 1;
        (*station_count)++;
    }
}

// fread implementation
double useFREAD(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return -1.0;
    }

    char line[256];
    int max_stations = 100000;
    StationStats *stations = calloc(max_stations, sizeof(StationStats));
    char **station_names = calloc(max_stations, sizeof(char *));
    int station_count = 0;

    clock_t start = clock();
    //getting the line using fgets which reads a line until a newline character appears
    while (fgets(line, sizeof(line), file)){
        process_line(line, stations, &station_count, station_names,max_stations);
    }
    clock_t end = clock();
    fclose(file);

    for (int i = 0; i < station_count; i++) {
        printf("%s min=%.2f mean=%.2f max=%.2f\n", station_names[i], 
               stations[i].min, stations[i].sum / stations[i].count, stations[i].max);
        free(station_names[i]);
    }
    double timeTaken=(double)(end - start)/ CLOCKS_PER_SEC;
    return timeTaken;
}

double useMMAP(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    struct stat stats;
    if (fstat(fd, &stats) == -1) {
        perror("Error getting file stats");
        close(fd);
        return -1.0;
    }

    //addr: A suggested starting address for the mapping. Often set to NULL to let the system choose.
    size_t file_size = stats.st_size;
    char *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return -1.0;
    }
    //Provides a hint to the kernel that the memory will be accessed sequentially. This can improve performance by optimizing memory access patterns.
    madvise(data, file_size, MADV_SEQUENTIAL);

    int max_stations = 100000;
    StationStats *stations = calloc(max_stations, sizeof(StationStats));
    char **station_names = calloc(max_stations, sizeof(char *));
    int station_count = 0;

    clock_t start = clock();

    // Process mapped data line by line
    char *line_start = data;
    char *line_end = NULL;

    while ((line_end = memchr(line_start, '\n', data + file_size - line_start))) {
        size_t line_length = line_end - line_start;

        //copying the line to line 
        char line[1000];
        if (line_length >= sizeof(line)) {
            fprintf(stderr, "Error: Line too long to process.\n");
            break;
        }
        //copying the line to line 
        memcpy(line, line_start, line_length);
        //adding NULL to the end of the line
        line[line_length] = '\0'; 

        process_line(line, stations, &station_count, station_names, max_stations);

        line_start = line_end + 1;  // Move to the next line
    }

    clock_t end = clock();

    munmap(data, file_size);
    close(fd);

    
    for (int i = 0; i < station_count; i++) {
        printf("%s min=%.2f mean=%.2f max=%.2f\n", station_names[i], 
               stations[i].min, stations[i].sum / stations[i].count, stations[i].max);
        free(station_names[i]);
    }
    free(stations);
    free(station_names);
    double timeTaken=(double)(end - start) / CLOCKS_PER_SEC;
    return timeTaken;
}


int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    printf("Processing with fread...\n");
    double timeTaken=useFREAD(argv[1]);
    if (timeTaken == -1.0) {
        fprintf(stderr, "Error occurred during fread processing.\n");
        return 1;
    }

    printf("\nProcessing with mmap...\n");
    double timeTaken2=useMMAP(argv[1]);
    if (timeTaken2 == -1.0) {
        fprintf(stderr, "Error occurred during mmap processing.\n");
        return 1;
    }

    printf("fread processing time: %.2f seconds\n", timeTaken);
    printf("mmap processing time: %.2f seconds\n",timeTaken2);

    return 0;
}
