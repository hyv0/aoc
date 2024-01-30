
#include <stdio.h>
#include <stdlib.h>

static void read();

static char* instructions[1000];
static unsigned int instrucionsCount = 0;

static struct NODE;
static struct NODE{
    char name[4];
    char left[4];
    char right[4];

    struct NODE* l;
    struct NODE* r;
};

static struct NODE nodes[10000];
static int nodecount = 0;

static struct NODE* a;
static struct NODE* z;


void day8(int part) {
    printf("Day 8 (2023)\n");
    read();
    printf("instructioncount = %d, nodecount = %d\n", instrucionsCount, nodecount);
    for(int i = 0; i < nodecount; i++) {
            for(int j = 0; j < nodecount; j++) {
                if(strcmp(nodes[i].left,nodes[j].name) == 0) {
                    nodes[i].l = &nodes[j];
                }
                if(strcmp(nodes[i].right,nodes[j].name) == 0) {
                    nodes[i].r = &nodes[j];
                }
            }
            if(strcmp(nodes[i].name,"AAA")==0) {
                a = &nodes[i];
            }
            if(strcmp(nodes[i].name,"ZZZ")==0) {
                z = &nodes[i];
            }
    }
    printf("pointers set\n");

    struct NODE* p = a;
    long i = 0;
    while(p != z) {
        //printf("NODE=%s. l = %s, r = %s. taking %c\n",p->name, p->left, p->right, instructions[i%instrucionsCount]);
        if(instructions[i%instrucionsCount] == 'L') {
            p = p->l;
        }else{
            p=p->r;
        }
        i++;
    }

    printf(" STEPS = %ld\n",i);


    struct NODE* manyNodes[10];
    int manyN = 0;
    for(int i = 0; i < nodecount; i++) {
        if(nodes[i].name[2] == 'A') {
            manyNodes[manyN] = &nodes[i];
            manyN++;
        }
    }
    printf("\nSimultanously executing %d nodes\n",manyN);
    //smart:

    long long cycleBegin = 0;
    long long cycleTimes [10];
    for(int i = 0; i < manyN; i++ ){
        struct NODE * node = manyNodes[i];
        struct NODE * firstTimeOverIndexZeroAgain = NULL;
        long long counter = 0;

        while(1) {
            int c = counter%instrucionsCount;
            counter++;
            if(instructions[c] == 'L') {
                node = node->l;
            }else{
                node=node->r;
            }
            if(node->name[2] == 'Z')
                printf("Z at c=%lld %d for %d\n", counter,c,i);
            if(c == 0 && counter > 10 ) {
                if(firstTimeOverIndexZeroAgain == NULL) {
                    firstTimeOverIndexZeroAgain = node;
                    cycleBegin= counter;
                }else{
                    if(node == firstTimeOverIndexZeroAgain) {
                        cycleTimes[i] = counter-cycleBegin;
                        printf("CYCLE AT %lld for %d   after %lld\n", counter, i, counter-cycleBegin);
                        break;
                    }
                }
            }

        }
    }

    // now find Least Common Multiples:
    long long mult = cycleTimes[0];
    while(1){
        int allOk = 1;
        for(int i = 1; i < manyN; i++) {
            if(mult % cycleTimes[i] != 0){
                    allOk = 0;
                break;
            }
        }
        if(allOk) break;
        mult += cycleTimes[0];
        if(mult < 0)
        printf("%lld\n", mult);
        if(mult  > 8811050362675)
        printf("%lld\n", mult);
    }

    printf("answer = %lld, -> index = %lld -> Z is at %lld\n", mult,mult+cycleBegin, mult+cycleBegin);
//8811050362677 to high
//8811050362676 too
//8811050362675 too
//8811050362678 wrong
//8811050362679 wrong
//8811050362680 wrong
//8811050362409      -< loesung

    return;

    // brutforce:

    long long unsigned int counter = 0;
    int highestIndex = manyN-1;
    while(1) {
        int oneNotZ = 0;
        int c = counter%instrucionsCount;
        for(int i = highestIndex; i >=0; i-- ){
            if(instructions[c] == 'L') {
                manyNodes[i] = manyNodes[i]->l;
            }else{
                manyNodes[i]=manyNodes[i]->r;
            }
            if(manyNodes[i]->name[2] != 'Z') {
                oneNotZ ++;
            }
          //  printf("%d %s\n",i,manyNodes[i]->name);

        }

        counter++;
        if(oneNotZ == 0) {
            break;
        }
        if(oneNotZ < 2 )
        printf("%12lld  notz=%d  > %lld%%%%    %s %s %s %s %s %s  %d\n",counter,oneNotZ,counter * 1000/ 8811050362409, manyNodes[0]->name,manyNodes[1]->name,manyNodes[2]->name,manyNodes[3]->name,manyNodes[4]->name,manyNodes[5]->name,  counter%instrucionsCount);
    }
    printf("SUM = %lld\n",counter);

    while(1);

}

static void read() {
    FILE* inputfile = fopen("./input/input8.txt","r");
    int c;
    int linenumber = 0;

    int charcount = 0;
    do
    {
        c = (char)fgetc(inputfile);
        if(c == '\n') {
            linenumber++;
        }
        if(linenumber == 0) {
            instructions[instrucionsCount] = c;
            instrucionsCount++;
        }
        if(linenumber > 1 &&( c >='A' && c<='Z' )|| ( c >='0' && c<='9' )) {
            if(charcount<3) {
                nodes[nodecount].name[charcount] = c;
            }else
            if(charcount<6) {
                nodes[nodecount].left[charcount-3] = c;
            }else
            if(charcount<9) {
                nodes[nodecount].right[charcount-6] = c;
            }
            charcount++;
        }
        if(charcount == 9) {
            charcount = 0;
            nodes[nodecount].name[3] = 0;
            nodes[nodecount].left[3] = 0;
            nodes[nodecount].right[3] = 0;
            nodes[nodecount].l = NULL;
            nodes[nodecount].r = NULL;
            nodecount++;

        }

    } while(c != EOF);
    fclose(inputfile);



}
