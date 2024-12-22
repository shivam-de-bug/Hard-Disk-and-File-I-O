/*For this question, you must use lseek system call to access file data randomly.
  You need to index and retrieve specific records from a binary file based on their byte
  location in the file. [ 15 points ]
  First, create a students.rec binary file using c. This file will contain the records of 5
  students. Each record contains the following fields:
  struct Student {
  int id; // Incremental student ID (4 bytes)
  char name[20]; // Student name (20 bytes)
  };
  Use fwrite function to write the records into the binary file. Do not submit the program used
  to create the students.rec file.
  Implement the following in Q3.c:
  ● Ask the user to input a student ID from 1 to 5.
  ● Open the students.rec file, and then use lseek to locate the student data in
  the file.
  ● Print the name of the found student.
  Input:
  ● ID of the student to be searched.
  E.g.
  Enter student id: 3
  Output:
  ● Name of the student corresponding to the input ID.
  E.g.
  Student name: John Doe
  Deliverables:
  - Q3.c, which contains the implementation of the program.
  - A students.rec file.
  - Makefile should compile this program.*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct Student{
    int id;
    char name[20];
};

int main() {
    char file[] = "students.rec";
    int student_id;

    printf("Enter student ID (1-5): ");
    scanf("%d", &student_id);

    if (student_id < 1 || student_id > 5) {
        printf("Invlaid Id\n");
        return 1;
    }
    //open in read only mode
    int fd = open(file, O_RDONLY);
    //
    if (fd == -1) {
        perror("Could not open file");
        return 1;
    }
    off_t offset = (student_id - 1) * sizeof(struct Student);
    int x=lseek(fd, offset, SEEK_SET);
    struct Student student;
    if (read(fd, &student, sizeof(struct Student)) != sizeof(struct Student)) {
        perror("Failed to read record");
        close(fd);
        return 1;
    }
    if (x == -1) {
        perror("Could not seek to position");
        close(fd);
        return 1;
    }
    printf("Student name: %s\n", student.name);
    close(fd);
    return 0;
}
