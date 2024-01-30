
#include <stdio.h>
#include <stdlib.h>
static void read() ;

static long long seeds[50];
static int lenghts[7] = {0};
static long long maps[7][50][3];

static int seedCount = 0;


static long long convert(int index, long long seed) {
    for(int i = 0; i < lenghts[index]; i++) {
        long long diff = seed - maps[index][i][1];
        if(diff >= 0 && diff < maps[index][i][2]) {
            return maps[index][i][0] + diff;
        }
    }
    return seed;
}

static void convertRanges(int index, long long rangeStart, long long rangeLength, long long * resultArr, int* resultArrLength) {

    int done = 0;
    while(1) {
    printf("remaing %lld\n", rangeLength);
        long long newRangeStart;
        long long newRangeLength;
        long long closestStart = 99999999999999999;
        int closestStartIndex= -1;
        for(int i = 0; i < lenghts[index]; i++) {
            long long diff = rangeStart - maps[index][i][1];
            if(diff < 0) {
                if(-diff < closestStart) {
                    closestStart = -diff;
                    closestStartIndex = i;
                }
            }
            if(diff >= 0 && diff < maps[index][i][2]) {
                newRangeStart = maps[index][i][0] + diff;
                newRangeLength =  maps[index][i][2] - diff;
                if(newRangeLength >= rangeLength) {
                    newRangeLength = rangeLength;
                    done = 1;
                }
                resultArr[(*resultArrLength)++] = newRangeStart;
                resultArr[(*resultArrLength)++] = newRangeLength;
                rangeLength -= resultArr[(*resultArrLength)-1];
                rangeStart += resultArr[(*resultArrLength)-1];
                closestStartIndex = -2;
                break;
            }
        }
        if(closestStartIndex != -2) {
            // insert a direct convertion:
            if(closestStartIndex != -1) {
                resultArr[(*resultArrLength)++] = rangeStart;
                resultArr[(*resultArrLength)++] = closestStart;
            }else{
                resultArr[(*resultArrLength)++] = rangeStart;
                resultArr[(*resultArrLength)++] = rangeLength;
                done = 1;

            }
            rangeLength -= resultArr[(*resultArrLength)-1];
            rangeStart += resultArr[(*resultArrLength)-1];
        }
        printf("added start=%lld length=%lld\n", resultArr[(*resultArrLength)-2], resultArr[(*resultArrLength)-1]);
        if(done) {
            break;
        }
    }
    printf("---\n");
}

void day5() {
    printf("Day 5 (2023)\n");
    read();
    printf("reading done\n");

    printf("%d seeds\n", seedCount);

    for(int i = 0; i < seedCount; i++) {
        printf("%lld ", seeds[i]);
    }

    for(int m = 0; m < 7; m++) {
        for(int l = 0; l < lenghts[m]; l++) {
            printf(" %lld %lld %lld\n", maps[m][l][0], maps[m][l][1], maps[m][l][2]);
        }
        printf("\n\n");
    }


    long long part1 = 99999999999999999;
    for(int i = 0; i < seedCount; i++) {
        long long last = seeds[i];
        for(int m = 0; m < 7; m++) {
            last = convert(m,last);
        }
        if(last < part1) {
            part1 = last;
        }
    }
    printf("part1 = %lld\n", part1);

    long long ranges[8][1000] = {0};
    for(int i = 0; i < seedCount; i++) {
        ranges[0][i] = seeds[i];
    }


    for(int m = 0; m < 7; m++) {
        int rangeCounter = 0;
        for(int i = 0; i < 1000; i+=2) {
            long long rangeStart = ranges[m][i];
            long long le = ranges[m][i+1];
            if(le == 0) continue;
            convertRanges(m, rangeStart, le, &ranges[m+1][0], &rangeCounter);
        }
    }

    long long part2 = 99999999999999999;
    for(int i = 0; i < 1000; i+=2) {
        long long s = ranges[7][i];
        if(s == 0) continue;
        if(s < part2)
            part2 = s;
    }
    printf("part2 = %lld\n", part2);





}


static char* skipBlank(char* ptr) {
    while(*ptr != ' ') ptr++;
    return ptr+1;
}


static void read() {
    FILE* inputfile = fopen("./input/input5.txt","r");

    int bufferLength = 255;
    char buffer[bufferLength];

    fgets(buffer, bufferLength, inputfile);
    char* ptr = buffer + 6;
    while(*ptr != '\n' && *ptr != '\0') {
        if(*ptr == ' ')ptr++;
        seeds[seedCount++] = atoll(ptr);
        while(*ptr >= '0' && *ptr <= '9') {
            ptr++;
        }
    }



    int mapCounter = 0;
    int lineCounter = 0;
    while(fgets(buffer, bufferLength, inputfile)) {
        if(buffer[0] >= '0' && buffer[0] <= '9') {
            maps[mapCounter][lineCounter][0] = atoll(buffer);
            maps[mapCounter][lineCounter][1] = atoll(skipBlank(buffer));
            maps[mapCounter][lineCounter][2] = atoll(skipBlank(skipBlank(buffer)));

            lineCounter++;
        }else{
            if(lineCounter != 0) {
                lenghts[mapCounter] = lineCounter;
                mapCounter++;
                lineCounter = 0;
            }
        }
    }
    lenghts[6] = lineCounter;



    fclose(inputfile);
}
