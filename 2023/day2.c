
#include <stdio.h>
#include <stdlib.h>
static void read() ;

static struct ROUND{
    int red;
    int green;
    int blue;
};

static struct INPUT{
    int id;
    int roundCount;
    struct ROUND rounds[20];

};

static struct INPUT input[120];
static int inputCount = 0;

void day2() {
    printf("Day 2 (2023)\n");
    read();
    printf("done\n");
    for(int i = 0; i < inputCount; i++) {
//        printf("Game %d (%d rounds):\n", input[i].id, input[i].roundCount);
        for(int j = 0; j < input[i].roundCount; j++) {
//            printf("    > red %d  green %d  %d blue\n", input[i].rounds[j].red, input[i].rounds[j].green, input[i].rounds[j].blue);
        }
    }

    int part1 = 0;
    for(int i = 0; i < inputCount; i++) {
        int possible = 1;
        for(int j = 0; j < input[i].roundCount; j++) {
            if((input[i].rounds[j].red > 12) || (input[i].rounds[j].green > 13) ||(input[i].rounds[j].blue > 14)){
                possible = 0;
            }


        }
        if(possible) {
            part1 += input[i].id;
        }
    }

    printf("part1 = %d\n", part1); // 3510 to high, 2308 too low

    long long part2 = 0;
    for(int i = 0; i < inputCount; i++) {
        int red = 0;
        int green = 0;
        int blue = 0;
        for(int j = 0; j < input[i].roundCount; j++) {
            if(input[i].rounds[j].green > green) {
                green = input[i].rounds[j].green;
            }
            if(input[i].rounds[j].blue > blue) {
                blue = input[i].rounds[j].blue;
            }
            if(input[i].rounds[j].red > red) {
                red = input[i].rounds[j].red;
            }
        }
        part2 += red * green * blue;
        printf("%d: %d*%d*%d = %d\n" ,i+1, red,green,blue, red*green*blue);
    }
    printf("part2 = %lld\n", part2); // 71455
}





static void read() {
    FILE* inputfile = fopen("./input/input2.txt","r");

    int bufferLength = 255;
    char buffer[bufferLength];

//    printf("\n\n\n");
    while(fgets(buffer, bufferLength, inputfile)) {
        int id = atoi(buffer+5);
        input[inputCount].id = id;
        int round = 0;
        char* ptr = buffer+8;
        while(1) {
//            printf("  <<< %s\n", ptr);
            if(*ptr==':')ptr++;
            int number = atoi(ptr);
            while(1) {
                ptr++;
                if(*ptr == 'r') {
                    input[inputCount].rounds[round].red = number;
                    break;
                }
                if(*ptr == 'g') {
                    input[inputCount].rounds[round].green = number;
                    break;
                }
                if(*ptr == 'b') {
                    input[inputCount].rounds[round].blue = number;
                    break;
                }
            }
            int roundFinished = 0;
            while(1) {
                if(*ptr == ',') {
                    ptr++;
                    break;
                }
                if(*ptr == ';') {
                    ptr++;
                    roundFinished = 1;
                    break;
                }
                if(*ptr == '\0' || *ptr == '\n' ||*ptr == 10||*ptr == EOF) {
                    roundFinished = 2;
                    break;
                }
                ptr++;
//                printf("%c %d", *ptr, *ptr);
            }
            if(roundFinished) {
                round++;
            }
            if(roundFinished == 2) {
                break;
            }
        }


        input[inputCount].roundCount = round;
        inputCount++;
    }


    fclose(inputfile);
}
