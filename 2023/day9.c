
#include <stdio.h>
#include <stdlib.h>

static int matrix[5000][5000];


static int calcDiff(int* in, int* out, int size) {

    int foundNotNull = 0;
    for(int i = 0; i < size - 1; i++) {
        int diffI = in[ i +1] - in[i];
        out[i] = diffI;
        if(diffI != 0) {
            foundNotNull = 1;
        }
    }
    return foundNotNull;
}

static void printArray(int* arr, int size) {
    for(int i = 0; i<size; i++) {
        //printf("[%4d] %d\n", i, arr[i]);
        printf("%5d ", arr[i]);
    }
    printf("\n");
}


static int solve(char* buffer, int part) {
        int goOn = 1;
        int size = 0;

        char* workingBuffer = buffer;
        while(goOn > 0) {
            //printf("[%d] LINE= [%s]", size, workingBuffer);
            goOn = sscanf(workingBuffer,"%d", &matrix[0][size]);
            size++;
            while(((*workingBuffer >= '0') && (*workingBuffer <= '9')) || *workingBuffer == '-') {
                workingBuffer++;
            }
            if(*workingBuffer == ' ')
            workingBuffer++;

        }


        int row = 1;
        int lastOutput = 1;
        while(lastOutput) {
            lastOutput = calcDiff(matrix[row-1], matrix[row], size-row);
            row++;
        }

 //       printf("SIZE=%d\n", size);

        int sum = 0;
        if(part == 0) {
            for(int i = 0; i < size-1; i++) {
                sum += matrix[size-i-2][i];
    //            printf(" ADDING [%d,%d] %d\n", size-i-2, i,matrix[size-i-2][i]);

            }
        }else{
            for(int i = 0; i < size-1; i++) {
                int val = matrix[size-i-2][0];
                sum = val - sum;
            }
        }
        for(int i = 0; i < 50; i++) {

            for(int j = 0; j < 50; j++) {
                matrix[i][j]= 0;
            }
        }
        return sum;
}

int day9(int part)
{
    printf("Day 9 (2023)\n");
    FILE* inputfile = fopen("./input/input9.txt","r");
    int in[5000];
    //int matrix[5000][5000];
    int bufferLength = 255;
    char buffer[bufferLength];

    int SUM = 0;

    while(fgets(buffer, bufferLength, inputfile)) {
        int sum = solve(buffer, part);
        SUM += sum;
        //printf("NEXT PREDICTED VALUE = %d\n", sum);

    }
    printf("SUM = %d", SUM);

    //while(goOn > 0) {
    //    goOn = fscanf(inputfile,"%d", &in[size]);
    //    size++;
    //}
    //printf("size = %d \n", size);
    fclose(inputfile);



    return 0;
}



/*
13 33 54 85 159 343 748 1539 2945 5269 8898 14313
  10 21 31 74  184 405  791
    11  10 43 110
       1  33
         31

*/
