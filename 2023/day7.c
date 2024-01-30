
#include <stdio.h>
#include <stdlib.h>
static void read();

static struct INPUT{
    char card[6];
    int bid;
    long score;
};

static struct INPUT input[1005] = {0};
static int inputCount = 0;

static int cmpfunc (const void * a, const void * b) {
   return ( *(char*)b - *(char*)a );
}


static int cmpfunc2 (const void * ap, const void * bp) {
    char a = *(char*)ap;
    char b = *(char*)bp;
    if(a == 'J' && b != 'J') {
        return 99999;
    }
    if(b == 'J' && a != 'J') {
        return -99999;
    }
   return b-a;
}

static int cmpfuncImput (const void * a, const void * b) {
   return ( ((struct INPUT*)a)->score - ((struct INPUT*)b)->score );
}

void day7(int part) {
    printf("Day 7 (2023)\n");
    read();
    printf("reading done\n");

    for(int i = 0; i < inputCount; i++) {
        long score = 0;
        char highest = 0;
        for(int j = 0; j < 5; j++) {
            char c = input[i].card[j];
            int number = 0;
            if(c <= '9' && c >= '0') {
                number = c - '0';
            }else{
                switch(c) {
                    case 'A': number = 14; break;
                    case 'K': number = 13; break;
                    case 'Q': number = 12; break;
                    case 'J': number = part == 1 ? 11 : 1; break;
                    case 'T': number = 10; break;
                    default: exit(55);
                }
            }
            if(number > highest) {
                highest = number;
            }
            score *= 16;
            score += number;
        }
        char buffer[5];
        buffer[0] = input[i].card[0];
        buffer[1] = input[i].card[1];
        buffer[2] = input[i].card[2];
        buffer[3] = input[i].card[3];
        buffer[4] = input[i].card[4];
        qsort(buffer, 5, sizeof(char), part == 1 ? cmpfunc : cmpfunc2);

        char occurances[5] = {0};
        char occurancesCount = 0;
        occurances[0] = 1;
        int j;
        for(j = 1; j < 5; j++) {
            if(buffer[j] == buffer[j-1]) {
                occurances[occurancesCount]++;
            }else{
                if(buffer[j] == 'J' && part == 2) {
                    break;
                }
                occurances[++occurancesCount] = 1;
            }
        }
        occurancesCount++;
        qsort(occurances, occurancesCount, sizeof(char), cmpfunc);
        occurances[0] += 5-j; // part2
        long offset = 16*16*16*16*16*16;
        printf("CARD %s: %d %d %d %d %d\n", input[i].card, occurances[0], occurances[1], occurances[2], occurances[3], occurances[4]);
        if(occurances[0] == 5) {
            score += offset * 7;
            printf("adding [5] %ld\n", offset*7);
        }else if(occurances[0] == 4) {
            score += offset * 6;
            printf("adding [4] %ld\n", offset*6);
        }else if(occurances[0] == 3) {
            score += offset * ((occurances[1] == 2) ? 5 : 4);
            printf("adding [3] %ld\n", offset * (occurances[1] == 2 ? 5 : 4));
        }else if(occurances[0] == 2) {
            score += offset * ((occurances[1] == 2) ? 3 : 2);
            printf("adding [2] %ld\n", offset * (occurances[1] == 2 ? 3 : 2));
        }else if(occurances[0] == 1) {
            //score += 16*16*16*16*16L * highest; // "high card" is not really good explained....
            //printf("adding [1] %ld\n", (16*16*16*16*16L * highest));
        }else{
            exit(22);
        }



        input[i].score = score;
    }


    qsort(input, inputCount, sizeof(struct INPUT), cmpfuncImput);

    long long part1 = 0;
    for(int i = 0; i < inputCount; i++) {
        part1 += input[i].bid * (i+1);
        printf("rank %4d: card %s: score: %6ld.   (bid:%3d)   sum=%lld\n",i+1, input[i].card, input[i].score, input[i].bid, part1);
    }
    printf("part1 = %lld\n", part1);//247729920,
}

static char* skipBlank(char* ptr) {
    while((*ptr) != ' ') ptr++;
    while((*ptr) == ' ') ptr++;
    return ptr;
}



static void read() {
    FILE* inputfile = fopen("./input/input7.txt","r");
    int bufferLength = 255;
    char buffer[bufferLength];

    while(fgets(buffer, bufferLength, inputfile)) {
        if(strlen(buffer) < 6) continue;
        input[inputCount].card[0] = buffer[0];
        input[inputCount].card[1] = buffer[1];
        input[inputCount].card[2] = buffer[2];
        input[inputCount].card[3] = buffer[3];
        input[inputCount].card[4] = buffer[4];
        input[inputCount++].bid = atoi(skipBlank(buffer));
    }


    fclose(inputfile);
}
