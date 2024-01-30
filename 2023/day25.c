
#include <stdio.h>
#include <stdlib.h>
static void read() ;

static struct INPUT{
    char name[4];
    char connectedNames[10][4];
    int connectedCount;
};

static struct NODE{
    char name[4];
    struct NODE* connectedTo[20];
    int* edgeMetaData[20];
    int connectedCount;
    int index;
};

static struct INPUT input[2000]={0};
static struct NODE nodes[2000];
static int metaData[8000] = {0};
static int metaDataCount = 0;
static int inputCount = 0;
static int nodeCount = 0;

static int check(int index, char* checked, int value) {
    checked[index] = value;
    int ret = 1;
    for(int i = nodes[index].connectedCount - 1; i >= 0; i--) {
        int ind = nodes[index].connectedTo[i]->index;
        if(*(nodes[index].edgeMetaData[i]) == 0 && checked[ind] == 0) {
            ret += check(ind, checked, value);
        }
    }
    return ret;
}

static int countGroups(int a, int b, int c) {
    char checked[1400] = {0};

    int reachedCount = check(0, checked, 1);
    if(reachedCount  < inputCount-3) {
        printf("REACHED COUNT = %d at %d %d %d\n", reachedCount, a, b, c);

        int groups = 1;
        while(reachedCount != inputCount) {
            for(int i = 0; i < inputCount; i++) {
                if(checked[i] == 0) {
                    reachedCount += check(i, checked, ++groups);
                }
            }
        }
        printf("groups = %d\n", groups);
    }
    //printf("%d\n", reachedCount);
}


void day25() {
    printf("Day 25 (2023)\n");
    read();
    printf("read %d\n", inputCount);

    /*
    for(int i = 0; i< inputCount; i++) {
        printf("%s:\n", input[i].name);
        for(int k = 0; k < input[i].connectedCount; k++ ) {
            printf("   %s\n", input[i].connectedNames[k]);
        }
    }
    */

// create node network:
    nodeCount = inputCount;
    //todo
    for(int i = 0; i< inputCount; i++) {
        nodes[i].name[0] = input[i].name[0];
        nodes[i].name[1] = input[i].name[1];
        nodes[i].name[2] = input[i].name[2];
        nodes[i].index = i;
        for(int k = 0; k < input[i].connectedCount; k++) {
            // connected i to name in k
            int found = 0;
            for(int m = 0; m < inputCount; m++) {
                if(strcmp(input[i].connectedNames[k], input[m].name)==0) {
                    //connected i to m

                    for(int c = 0; c < nodes[i].connectedCount; c++) {
                        if(nodes[i].connectedTo[c] == &nodes[m]) {
                            printf("DUPLICATE\n");
                        }
                    }
                    for(int c = 0; c < nodes[m].connectedCount; c++) {
                        if(nodes[m].connectedTo[c] == &nodes[i]) {
                            printf("DUPLICATE\n");
                        }
                    }
                    nodes[i].connectedTo[nodes[i].connectedCount] = &nodes[m];
                    nodes[m].connectedTo[nodes[m].connectedCount] = &nodes[i];
                    nodes[i].edgeMetaData[nodes[i].connectedCount++] = &metaData[metaDataCount];
                    nodes[m].edgeMetaData[nodes[m].connectedCount++] = &metaData[metaDataCount++];
                    found = 1;
                    break;
                }
            }
            if(found == 0) {
//                nodes[]
            }
        }
    }
    printf("%d nodes and %d edges\n", inputCount, metaDataCount);


    for(int a = 0; a < metaDataCount; a++) {
        metaData[a] = 1;
        for(int b = a+1; b < metaDataCount; b++) {
            metaData[b] = 1;
            for(int c = b+1; c < metaDataCount; c++) {
                metaData[c] = 1;
                // check if two groups exist:
                if(countGroups(a,b,c)==2) {
                    printf("FOUND 2 groups\n");
                }
                metaData[c] = 0;
            }
            metaData[b] = 0;
        }
        metaData[a] = 0;
        printf("%d\n", a);
    }




    printf("done\n");





}





static void read() {
    FILE* inputfile = fopen("./input/input25.txt","r");

    int bufferLength = 255;
    char buffer[bufferLength];

    while(fgets(buffer, bufferLength, inputfile)) {
        input[inputCount].name[0] = buffer[0];
        input[inputCount].name[1] = buffer[1];
        input[inputCount].name[2] = buffer[2];

        int index = 5;
        while(buffer[index] >= 'a' && buffer[index] <= 'z') {
            input[inputCount].connectedNames[input[inputCount].connectedCount][0] = buffer[index++];
            input[inputCount].connectedNames[input[inputCount].connectedCount][1] = buffer[index++];
            input[inputCount].connectedNames[input[inputCount].connectedCount++][2] = buffer[index++];
            if(buffer[index] == ' ')index++;
        }


        inputCount++;
    }


    fclose(inputfile);
}
