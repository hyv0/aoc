
#include <stdio.h>
#include <stdlib.h>
static void read() ;


static struct INPUT{
    int id;
    int winning[10];
    int my[25];
};

static struct INPUT input[300];
static int inputCount = 0;

void day4() {
    printf("Day 4 (2023)\n");
    read();
    printf("reading done\n");
    for(int i = 0; i < inputCount; i++) {
        printf("[%4d]  >  " , input[i].id);
        for(int j = 0; j < 10; j ++) {
            printf("%2d ", input[i].winning[j]);
        }
        printf(" ### ");
        for(int j = 0; j < 25; j ++) {
            printf("%2d ", input[i].my[j]);
        }
        printf("\n");
    }

    int part1 = 0;
    for(int i = 0; i < inputCount; i++) {
        int score = 1;
        for(int j = 0; j < 10; j++) {
            for(int k = 0; k < 25; k++) {
                if(input[i].winning[j] == input[i].my[k]){
                    score *= 2;
                }
            }
        }
//        printf("score=%d\n", score);
        part1 += score /2;
    }
    printf("part1 = %d\n\n", part1);


    int copies[300] = {0};
    int part2 = 0;
    for(int i = 0; i < inputCount; i++) {
        int score = 0;
        for(int j = 0; j < 10; j++) {
            for(int k = 0; k < 25; k++) {
                if(input[i].winning[j] == input[i].my[k]){
                    score++;
                }
            }
        }
        int count = copies[i] + 1;
        for(int j = 0; j < score; j++) {
            copies[i+j+1]+=count;
        }
        part2 += count;
    }
    printf("part2 = %d", part2);


}





static void read() {
    FILE* inputfile = fopen("./input/input4.txt","r");

    int bufferLength = 255;
    char buffer[bufferLength];

    while(fgets(buffer, bufferLength, inputfile)) {

        input[inputCount].id = atoi(buffer+5);
        for(int i = 0; i < 10; i++) {
            input[inputCount].winning[i] = atoi(buffer+10+3*i);
        }
        for(int i = 0; i < 25; i++) {
            input[inputCount].my[i] = atoi(buffer+42+3*i);
        }
        inputCount++;
    }


    fclose(inputfile);
}
