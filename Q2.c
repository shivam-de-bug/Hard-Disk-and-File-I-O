/*Write a C program to simulate the Shortest Seek Time First (SSTF) disk scheduling
  algorithm and calculate both seek time and rotational latency for a given set of requests.
  Assume the disk's rotational speed is 72 RPM, the seek time per track is 2 ms, and the
  initial position of the head is 100. There are 5 tracks and 100 sectors per track. Tracks
  are uniformly aligned, i.e., when tracks change, head position changes as 125 -> 225 or
  125 -> 25.
  Input:
  The disk request queue of length 4, containing the disk sectors requested.
  E.g.
  Disk requests: 78 289 21 495
  Output:
  ● Total seek time and rotational latency of each request.
  E.g.
  Seek Time 1: 0 ms
  Rotational Latency 1: 0.11 ms
  Seek Time 2: 4 ms
  Rotational Latency 2: 0.5 ms
  ...
  Deliverables:

  - Q2.c which contains the implementation of this program.
  - Makefile should compile this program.*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define TRACK_COUNT 5
#define ROTATION_TIME_MS 833.3//time taken to complete one rotation in ms
#define SEEK_TIME_PER_TRACK_MS 2

// Disk request structure
typedef struct {
    int track;
    int sector;
} DiskRequest;

// Function to calculate track and sector from sector number
DiskRequest calculate_track_and_sector(int sector_number) {
    DiskRequest req;
    req.track = sector_number / 100;
    req.sector = sector_number % 100;
    return req;
}

// Function to calculate rotational latency
double calculate_rotational_latency(int current_sector, int target_sector) {
    int sector_distance = abs(current_sector - target_sector);
    return (sector_distance / 100.0) * ROTATION_TIME_MS;
}

// Function to find the nearest request based on track distance
int find_nearest_request(DiskRequest *requests, int current_track, int *processed) {
    int min_distance = TRACK_COUNT; // Maximum possible distance(5)
    int closestRequest = -1;

    for (int i = 0; i < 4; i++) {
        if (!processed[i]) {
            int distance = abs(requests[i].track - current_track); //
            if (distance < min_distance) {
                min_distance = distance;
                closestRequest = i;
            }
        }
    }
    return closestRequest;
}

int main() {
    int request_sectors[4]; // Example input 78, 289, 21, 495
    printf("Enter 4 requests: ");
    for(int i=0;i<4;i++){
        scanf("%d",&request_sectors[i]);
    }
    // int request_count = sizeof(request_sectors) / sizeof(request_sectors[0]);
    DiskRequest requests[4];
    int processed[4]; // Track processed requests
    double totalSeekTime = 0, totalRotationLatencyTime = 0;

    // Initialize request structures
    for (int i = 0; i < 4; i++) {
        DiskRequest req;
        req.track =  request_sectors[i]/ 100;
        req.sector = request_sectors[i] % 100;
        requests[i] = req;
        processed[i] = 0; // initially nothing is processed
    }

    // Initial position of the head is 100 and track numbers start from 0
    int current_track = 1, current_sector = 0;

    printf("Disk Scheduling: Shortest Seek Time First (SSTF)\n");
    for (int i = 0; i < 4; i++) {
        // Find the nearest request
        int closestRequest = find_nearest_request(requests, current_track, processed);
        if (closestRequest == -1) break; // No unprocessed requests

        // Calculate seek time and rotational latency
        int target_track = requests[closestRequest].track;
        int target_sector = requests[closestRequest].sector;

        double seek_time = SEEK_TIME_PER_TRACK_MS * abs(target_track - current_track);
        double rotational_latency = (abs(target_sector-current_sector)/100.0)*ROTATION_TIME_MS;
        totalSeekTime += seek_time;
        totalRotationLatencyTime += rotational_latency;

        printf("Request %d: Track %d, Sector %d\n", i + 1, target_track, target_sector);
        printf("Seek Time: %.2f ms, Rotational Latency: %.2f ms\n", seek_time, rotational_latency);

        //needle goes to the new position
        current_track = target_track;
        current_sector = target_sector;
        processed[closestRequest] = 1;
    }

    printf("\nTotal Seek Time: %.2f ms\n", totalSeekTime);
    printf("Total Rotational Latency: %.2f ms\n", totalRotationLatencyTime);

    return 0;
}
