
#include <stdio.h>
#include <stdlib.h>

struct PART{
    int type;
    int firstGear;
};

static int matrix[1000][1000];
static struct PART* matrixInteresstingArea[1000][1000]; // holds references to gearRatioArray items // in the beginning it just was an int-matrix. But for part 2 we need more information
static struct PART gearRatioArray[10000];
static int gearRatioArrayIndex = 0;
static int wi=0;
static int wj=0;

static void read();
static void printm(int m[1000][1000]);
static void mark(int i , int j, struct PART* symbol);

void day3() {
    read();
    printm(matrix);


    for(int i = 0; i < 1000; i++) {
        for(int j = 0; j < 1000; j++) {
            matrixInteresstingArea[i][j] = NULL;
        }
    }

    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
            int c = matrix[i][j];
            if((c < '0' || c> '9') && c != '.') {
                struct PART* ratioAddress = &gearRatioArray[gearRatioArrayIndex];
                gearRatioArray[gearRatioArrayIndex].type = '*';
                gearRatioArray[gearRatioArrayIndex].firstGear = 0;
                gearRatioArrayIndex++;
                mark(i, j+1, ratioAddress);
                mark(i, j-1, ratioAddress);
                mark(i-1, j+1, ratioAddress);
                mark(i-1, j, ratioAddress);
                mark(i-1, j-1, ratioAddress);
                mark(i+1, j+1, ratioAddress);
                mark(i+1, j, ratioAddress);
                mark(i+1, j-1, ratioAddress);
            }
        }

    }

    long sum = 0;
    for(int i = 0; i < wi; i++) {
        int number = 0;
        int addup = 0;
        for(int j = 0; j <= wj; j++) {


            if(matrix[i][j] >= '0' && matrix[i][j] <='9') {
                number *= 10;
                number += matrix[i][j]-'0';
                if(matrixInteresstingArea[i][j] != NULL) {
                    addup = 1;
                }
            }else{
                if(addup) {
                    addup = 0;
                    if(number != 0) {
                        sum += number;
                    }
                }
                number = 0;
            }

        }
    }

    printf("SUM = %d\n", sum);

    long ratiosum = 0;
        for(int i = 0; i < wi; i++) {
        int number = 0;
        struct PART* addup = NULL;
        for(int j = 0; j <= wj; j++) {
            if(matrix[i][j] >= '0' && matrix[i][j] <='9') {
                number *= 10;
                number += matrix[i][j]-'0';
                if(matrixInteresstingArea[i][j] != NULL && matrixInteresstingArea[i][j]->type == '*') {
                    addup = matrixInteresstingArea[i][j];
                }
            }else{
                if(addup != NULL) {
                    if(number != 0) {
                        if(addup->firstGear == 0) {
                            addup->firstGear = number;
                        }else{
                            ratiosum += addup->firstGear * number;
                        }
                    }
                    addup = NULL;
                }
                number = 0;
            }
        }
    }

    printf("RATIOSUM = %ld\n", ratiosum);

}


static void mark(int i , int j, struct PART* symbol) {
    if(i >= 0 && j >= 0 && i < wi && j < wj) {
        if(matrixInteresstingArea[i][j] == NULL || symbol->type == '*')
            matrixInteresstingArea[i][j] = symbol;
    }
}


static void read() {
    FILE* inputfile = fopen("./input/input3.txt","r");
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

static void printm(int m[1000][1000]) {
    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
            printf("%c", m[i][j]);
        }
        printf("\n");
    }
}
