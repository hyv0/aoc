
#include <stdio.h>
#include <stdlib.h>

static void read();
static void printm(int m[200][200]);
static void findS(int* ip, int* jp);

static int matrix[200][200];
static int matrixPath[200][200];
static int wi = 0;
static int wj = 0;
static int si = 0;
static int sj = 0;

void day10() {
    printf("Day 10 (2023)\n");
    read();
    printf("READ input\n");
    //printm(matrix);
    findS(&si, &sj);
    printf(">> S at %d %d\n", si, sj);

    int lastDirection = -1; // 1 = north, 2 = east, 3 = south, 4 = west
    int i = si;
    int j = sj;
    int counter = 0;

    if(matrix[i+1][j]== 'J' || matrix[i+1][j]== 'L') {
        lastDirection = 3;
    }else if(matrix[i][j+1]== '7' || matrix[i+1][j]== 'J') {
        lastDirection = 2;
    }else if(matrix[i-1][j]== '7' || matrix[i+1][j]== 'F') {
        lastDirection = 1;
    }else if(matrix[i][j-1]== 'L' || matrix[i+1][j]== 'F') {
        lastDirection = 4;
    }


    while(1) {
        counter++;
        int c = matrix[i][j];
        matrixPath[i][j] = c;
        int nextc;
        if(lastDirection == 1) {
            i--;
        }else if(lastDirection == 2) {
            j++;
        }else if(lastDirection == 3) {
            i++;
        }else if(lastDirection == 4) {
            j--;
        }
        int nextChar = matrix[i][j];
        if(nextChar == 'S') {
            break;
        }

        if(nextChar=='L') {
            lastDirection = (lastDirection == 3) ? 2 : 1;
        }else if(nextChar=='7') {
            lastDirection = (lastDirection == 1) ? 4 : 3;
        }else if(nextChar=='F') {
            lastDirection = (lastDirection == 1) ? 2 : 3;
        }else if(nextChar=='J') {
            lastDirection = (lastDirection == 3) ? 4 : 1;
        }//else: | and | dont change direction
    }

    printf("\n\n");
    printm(matrixPath);
    printf("LENGTH = %d. /2=%d\n", counter, counter/2);

    int area = 0;
    for(int i = 0; i < wi; i++) {
        int isInside = 0;
        int expectedDir = 0;
        for(int j = 0; j < wj; j++) {
            int isInsideBefore = isInside;

            int c = matrixPath[i][j];
            if(c == '|') {
                isInside = !isInside;
            }
            if(c == 'J') {
                if(expectedDir == 1) {
                    isInside = !isInside;
                }else{
                    expectedDir = 0;
                }
            }
            if(c == 'F') {
                expectedDir = 1;
            }
            if(c == 'L') {
                expectedDir = 3;
            }
            if(c == '7') {
                if(expectedDir == 3) {
                    isInside = !isInside;
                }else{
                    expectedDir = 0;
                }
            }
            if(isInside && isInsideBefore && c == 0) {
                area++;
                //printf("LINE=%d\n", i);
                //printf(" AREA[y=%d,x=%d=%c]", i, j, c);
            }
        }
        //printf("\n");
        if(area != 0) {
            //return;
        }
    }

    printf("AREA = %d\n", area);
}

static void read() {
    FILE* inputfile = fopen("./input/input10.txt","r");
    int c;
    int i = 0;
    int j = 0;
    do
    {
        c = (char)fgetc(inputfile);
        matrix[i][j] = c;
        if(c == '\n') {
            i++;
            wj = j;
            j = 0;
            wi++;
        }else{
            j++;
        }
    } while(c != EOF);
    fclose(inputfile);
}

static void printm(int m[200][200]) {
    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
            printf("%c", m[i][j]);
        }
        printf("\n");
    }
}

static void findS(int* ip, int* jp) {
    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
            if(matrix[i][j]=='S') {
                *ip = i;
                *jp = j;
                return;
            }
        }
    }
}
