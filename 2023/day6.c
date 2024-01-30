
#include <stdio.h>
#include <stdlib.h>
static void read() ;

static int distances[4];
static int times[4];


void day6() {
    printf("Day 6 (2023)\n");
    read();
    printf("reading done\n");

    for(int i = 0; i < 4 ; i++) {
        printf(">> in %d ms distance = %d\n", times[i], distances[i]);
    }

    int part1 = 1;
    for(int i = 0; i < 4 ; i++) {
        int c = 0;
        for(int t = 0; t < times[i]; t++) {
            if(t * (times[i] - t) > distances[i]) {
                c++;
            }
        }
        part1 *= c;
    }

    printf("part1 = %d\n\n", part1);

    char buffer[100];
    sprintf(buffer, "%d%d%d%d", times[0],times[1],times[2],times[3]);
    long long time = atoll(buffer);
    sprintf(buffer, "%d%d%d%d", distances[0],distances[1],distances[2],distances[3]);
    long long distance = atoll(buffer);

    printf("p2: %lld   %lld\n", distance, time);

    long long part2 = 0;
    for(long long t = 0; t < time; t++) {
        if(t * (time - t) > distance) {
            part2++;
        }
    }
    printf("part2 = %lld\n", part2);

}


static char* skipBlank(char* ptr) {
    while((*ptr) != ' ') ptr++;
    while((*ptr) == ' ') ptr++;
    return ptr;
}



static void read() {
    FILE* inputfile = fopen("./input/input6.txt","r");

    int bufferLength = 255;
    char buffer[bufferLength];

    fgets(buffer, bufferLength, inputfile);
    char* start = buffer;
    for(int i = 0; i < 4; i++) {
        start = skipBlank(start);
        times[i] = atoi(start);
    }

    fgets(buffer, bufferLength, inputfile);
    start = buffer;
    for(int i = 0; i < 4; i++) {
        start = skipBlank(start);
        distances[i] = atoi(start);
    }


    fclose(inputfile);
}
