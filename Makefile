
CC = gcc
CFLAGS = -Wall 

PROGRAMS = Q1 Q2 Q3 Q4
all: $(PROGRAMS)


Q1: Q1.c
	$(CC) $(CFLAGS) -o q1 Q1.c

Q2: Q2.c
	$(CC) $(CFLAGS) -o q2 Q2.c

Q3: Q3.c
	$(CC) $(CFLAGS) -o q3 Q3.c

Q4: Q4.c
	$(CC) $(CFLAGS) -o q4 Q4.c

clean:
	rm -f $(PROGRAMS)

