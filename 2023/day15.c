
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void read();
static struct PROBLEM{
    char str[50];
    int l;
    int isDash;
    int numberBehind;
};
static struct PROBLEM problems[7000];
static int problemCount = 0;

#define BOXSIZE 200
static struct PROBLEM* boxes[256][BOXSIZE];

void day15(int part) {
    printf("Day 15 (2023)\n");
    read();
    printf("READ input (%d)\n", problemCount);

    long long ret = 0;
    for(int i = 0; i < problemCount; i++) {
        struct PROBLEM* p = &problems[i];
        long long v = 0;
        for(int j = 0; j < p->l; j++) {
            v += p->str[j];
            v *= 17;
            v = v % 256;
            //printf("CHAR %c %d -> %d\n",p->str[j],p->str[j],v );

        }
        ret += v;
    }

    for(int i = 0; i < 256; i++) {
        for(int j = 0; j < BOXSIZE; j++) {
            boxes[i][j] =NULL;
        }
    }

   for(int i = 0; i < problemCount; i++) {
        struct PROBLEM* p = &problems[i];
        long long v = 0;
        int l = p->l;
        if(p->isDash) {
            l--;
        }else{
            l-=2;
        }
        for(int j = 0; j < l; j++) {
            v += p->str[j];
            v *= 17;
            v = v % 256;
        }

        if(p->isDash) {
            int found = 0;
            for(int j = 0; j < BOXSIZE; j++) {
                struct PROBLEM * moving = boxes[v][j];
                if(found == 0) {
                    if(moving != NULL && strncmp(boxes[v][j]->str, p->str, l)==0) {
                        found = 1;
                    }
                }else{
                    boxes[v][j-1] = boxes[v][j];
                }
            }
        }else{
            int found = 0;
            int firstNullIndex = -1;
            for(int j = 0; j < BOXSIZE; j++) {
                struct PROBLEM * moving = boxes[v][j];
                if(moving != NULL && strncmp(moving->str, p->str, l)==0) {
                    boxes[v][j] = p;
                    found = 1;
                    break;
                }
                if(moving == NULL && firstNullIndex == -1) {
                    firstNullIndex = j;
                }
            }
            if(found == 0) {
                boxes[v][firstNullIndex] = p;
            }

        }
    }

    long long sum2 = 0;
    for(int i = 0; i < 256; i++) {
        for(int j = 0; j < BOXSIZE; j++) {
            struct PROBLEM * p = boxes[i][j];
            if(p != NULL) {
                long long v = 0;
                v = (i+1) * (j+1) * p->numberBehind;
                sum2 += v;
            }
        }
    }

    /*
    for(int i = 0; i < 255; i++) {
        for(int j = 0; j < BOXSIZE; j++) {
            if(boxes[i][j]) {
                printf("%d",boxes[i][j]->numberBehind);
            }else{
            printf(".");
            }
        }
            printf("\n");
    }
    */


    printf("SUM = %lld\n SUM2 = %lld\n", ret, sum2); // SUM2=246487 (255 instead of 256... )


}

static void read() {
    FILE* inputfile = fopen("./input/input15.txt","r");
    int c;
    int i = 0;
    do
    {
        c = (char)fgetc(inputfile);
        problems[problemCount].str[i] = c;
        i++;
        if(c == '\n' || c== ',') {
            problems[problemCount].str[i-1] = '\0';
            problems[problemCount].l = i-1;
            problemCount++;
            i = 0;
        }
        if(c=='-'){
            problems[problemCount].isDash = 1;
        }
        if(c >= '0' && c<= '9') {
            problems[problemCount].numberBehind = c - '0';
        }
    } while(c != EOF);
    fclose(inputfile);



}
