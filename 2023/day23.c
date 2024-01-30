
#include <stdio.h>
#include <stdlib.h>
static void read() ;
static char matrix1[200][200];





static int wj = 0;
static int wi = 0;

static int pathLength[200][200] = {0};

static int allowedDirections(int i, int j) {
    char c = matrix1[i][j];
    int allowed = 0;
    if(i > 0) {
        if(matrix1[i-1][j]=='.' || matrix1[i-1][j]=='^'){
            allowed |= 0x1;
        }
    }
    if(j > 0) {
        if(matrix1[i][j-1]=='.' || matrix1[i][j-1]=='<'){
            allowed |= 0x8;
        }
    }
    if(i < wi-1) {
        if(matrix1[i+1][j]=='.' || matrix1[i+1][j]=='v'){
            allowed |= 0x4;
        }
    }
    if(j < wj-1) {
        if(matrix1[i][j+1]=='.' || matrix1[i][j+1]=='>'){
            allowed |= 0x2;
        }
    }
    if(c == '>') {
        return 0x2;
    }
    if(c == '<') {
        return 0x8;
    }
    if(c == 'v') {
        return 0x4;
    }
    if(c == '^') {
        return 0x1;
    }
    return allowed;
}

static void setCords(int direction, int* i, int* j){
    switch(direction){
        case 0x1: (*i)--;break;
        case 0x2: (*j)++;break;
        case 0x4: (*i)++;break;
        case 0x8: (*j)--;break;
    }
}


static int lastL = 0;

static int search(int i, int j, int pathL, char (* visited)[150][150]){
    again:
    if(pathL > lastL) {
        lastL = pathL;
        printf("[%d][%d] l=%d  visited=%d\n",i,j,pathL, (*visited)[i][j]);
    }

    if((*visited)[i][j] == 1) {
        return 0;
    }
    if(i == wi-1) {
        return pathL;
    }
    (*visited)[i][j] = 1;
    pathL++;
    int nextDir = allowedDirections(i,j);
    int c = 0;
    if(nextDir & 0x1) {
        if((*visited)[i-1][j] == 0) {
            c++;
        }else{
            nextDir -= 0x1;
        }
    }
    if(nextDir & 0x2) {
        if((*visited)[i][j+1] == 0) {
            c++;
        }else{
            nextDir -= 0x2;
        }
    }if(nextDir & 0x4) {
        if((*visited)[i+1][j] == 0) {
            c++;
        }else{
            nextDir -= 0x4;
        }
    }if(nextDir & 0x8) {
        if((*visited)[i][j-1] == 0) {
            c++;
        }else{
            nextDir -= 0x8;
        }
    }
    if(c == 0) return 0;
    if(c==1) {
        setCords(nextDir, &i, &j);
        goto again;
    }else{
        int highest = 0;
        char visited2[150][150];
        if(nextDir & 0x1) {
            memcpy(&visited2, visited, 150*150);
            int result = search(i-1,j,pathL, &visited2);
            if(result > highest) highest = result;
        }
        if(nextDir & 0x2) {
            memcpy(&visited2, visited, 150*150);
            int result = search(i,j+1,pathL, &visited2);
            if(result > highest) highest = result;
        }
        if(nextDir & 0x4) {
            memcpy(&visited2, visited, 150*150);
            int result = search(i+1,j,pathL, &visited2);
            if(result > highest) highest = result;
        }
        if(nextDir & 0x8) {
            memcpy(&visited2, visited, 150*150);
            int result = search(i,j-1,pathL, &visited2);
            if(result > highest) highest = result;
        }
        return highest;
    }
}


void day23(int part2) {
    printf("Day 23 (2023)\n");
    read(part2);

    printf("read %d %d\n", wi, wj);

    char u[150][150] = {0};

    int longestP = search(0,1,0, &u);

    printf("longestPath = %d\n", longestP); // part2 takes a bit longer to calculate. BUt not as long as reworking all the code

}





static void read(int replace) {
    FILE* inputfile = fopen("./input/input23.txt","r");
    int c;
    int i = 0;
    int j = 0;
    do
    {
        c = (char)fgetc(inputfile);
        if(replace){
            if(c=='<' || c=='>' || c=='v' || c=='^') {
                c = '.';
            }
        }
        matrix1[i][j] = c;
        if(c == '\n') {
            wj = j;
            i++;
            j = 0;
        }else{
            j++;
        }
    } while(c != EOF);
    wi = i;
    fclose(inputfile);
}



