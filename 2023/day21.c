
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void read();

static int wi = 0;
static int wj = 0;
static int matrix[100][135][135]={0};


void day21(int part) {
    printf("Day 21 (2023)\n");
    read();
    printf("GRID SIZE = %d %d\n", wi, wj);
    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {

        }
    }

    int maxSteps = 64;
    for(int step = 0; step < maxSteps; step++ ) {
        for(int i = 0; i < wi; i++) {
            for(int j = 0; j < wj; j++) {
                int c = matrix[step][i][j];
                if(c==-1){
                    matrix[step+1][i][j]=-1;
                }
                if(c==1){
                    if(i>0 && matrix[step][i-1][j]>=0) {
                        matrix[step+1][i-1][j]=1;
                    }
                    if(j>0 && matrix[step][i][j-1]>=0) {
                        matrix[step+1][i][j-1]=1;
                    }
                    if(i<wi-1 && matrix[step][i+1][j]>=0) {
                        matrix[step+1][i+1][j]=1;
                    }
                    if(j<wj-1 && matrix[step][i][j+1]>=0) {
                        matrix[step+1][i][j+1]=1;
                    }
                }
            }
        }
    }
    long counter = 0;
    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
            if(matrix[maxSteps][i][j] == 1) {
                counter++;
                printf("o");
            }else if(matrix[0][i][j] == -1) {
                printf("X");

            }else{
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("AFTER %d steps: %ld possible fields\n", maxSteps, counter);




}

static void read() {
    FILE* inputfile = fopen("./input/input21.txt","r");
    int c;
    int i = 0;
    int j = 0;
    do
    {
        c = (char)fgetc(inputfile);
        if(c == '#') {
            matrix[0][i][j] = -1;
        }
        if(c == '.') {
            matrix[0][i][j] = 0;
        }
        if(c=='S') {
            matrix[0][i][j] = 1;
        }
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
