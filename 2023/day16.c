
#include <stdio.h>
#include <stdlib.h>

static void read();

static int wi = 0;
static int wj = 0;
static int matrix[200][200];
static int beamDir[200][200]; // bit 0: up, bit 1: right, bi2 down, bit 4: left




static struct BEAMHEAD{
    int i;
    int j ;
    int direction;
};


static struct BEAMHEAD beams[100];



static void  workOnBeam(int i, int j , int direction) {



      if(i < 0 || j < 0) return;
      if( i >= wi  || j >= wj) return;
      if((beamDir[i][j] & direction) != 0) return;                // this bracket cost me 45 minutes....




    beamDir[i][j] |= direction;


        if(matrix[i][j] == '.') {
            switch(direction){
                case 1:
                    workOnBeam(i-1,j,1); break;
                case 2:
                    workOnBeam(i,j+1,2); break;
                case 4:
                    workOnBeam(i+1,j,4); break;
                case 8:
                    workOnBeam(i,j-1,8); break;
                default:
                    printf("\n\nERROR %d\n", direction);
            }
        }else
        if(matrix[i][j] == '|') {
            switch(direction){
                case 1:
                    workOnBeam(i-1,j,1); break;
                case 2:
                case 8:
                    workOnBeam(i+1,j,4);
                    workOnBeam(i-1,j,1); break;
                case 4:
                    workOnBeam(i+1,j,4); break;
                default:
                    printf("\n\nERROR %d\n", direction);
            }
        }else
        if(matrix[i][j] == '-') {
            switch(direction){
                case 1:
                case 4:
                    workOnBeam(i,j+1,2);
                    workOnBeam(i,j-1,8); break;
                case 2:
                    workOnBeam(i,j+1,2); break;
                case 8:
                    workOnBeam(i,j-1,8); break;
                default:
                    printf("\n\nERROR %d\n", direction);
            }
        }else
        if(matrix[i][j] == '/') {
            switch(direction){
                case 1:
                    workOnBeam(i,j+1,2); break;
                case 2:
                    workOnBeam(i-1,j,1); break;
                case 4:
                    workOnBeam(i,j-1,8); break;
                case 8:
                    workOnBeam(i+1,j,4); break;
                default:
                    printf("\n\nERROR %d\n", direction);
            }
        }else
        if(matrix[i][j] == '\\') {
            switch(direction){
                case 1:
                    workOnBeam(i,j-1,8); break;
                case 2:
                    workOnBeam(i+1,j,4); break;
                case 4:
                    workOnBeam(i,j+1,2); break;
                case 8:
                    workOnBeam(i-1,j,1); break;
                default:
                    printf("\n\nERROR %d\n", direction);
            }
        }else{
            printf("\n\n\nERROR %d %d!", i,j);
        }

    }

static long getSum(int i, int j , int dir) {

    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
            beamDir[i][j] = 0;
        }
    }
    workOnBeam(i,j,dir);

    long sum = 0;

    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
            if(beamDir[i][j] != 0) {
                sum++;
            }
        }
    }
    return sum;
}

void day16() {
    printf("Day 16 (2023)\n");
    read();
    printf("GRID SIZE = %d %d\n", wi, wj);


    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
            beamDir[i][j] = 0;
        }
    }

    long highestSum = 0;

    for(int i = 0; i < wi; i++) {
        long s = getSum(i,0,2);
        if( s > highestSum) {
            highestSum = s;
        }
        s = getSum(i,wj-1,8);
        if( s > highestSum) {
            highestSum = s;
        }
    }
    for(int j = 0; j < wj; j++) {
        long s = getSum(0,j,4);
        if( s > highestSum) {
            highestSum = s;
        }
        s = getSum(wi-1,j,1);
        if( s > highestSum) {
            highestSum = s;
        }
    }




    printf("RESULT = %ld %ld\n",getSum(0,0,2), highestSum); // 2272

}

static void read() {
    FILE* inputfile = fopen("./input/input16.txt","r");
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
