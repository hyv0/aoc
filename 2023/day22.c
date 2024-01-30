
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void read();


static struct POINT{
    int x;
    int y;
    int z;
};

static struct INPUT{
    struct POINT s;
    struct POINT e;
    struct INPUT* restsOn[100];
    int restsOnSize;
    struct INPUT* supports[100];
    int supportsSize;
    int index;
};

static struct INPUT input[2000];
static int inputLength = 0;

static int wx;
static int wy;
static int wz;
static struct INPUT* (*matrix)[20][20][400] = {0};
static struct INPUT* matrixSum[400][20] = {0};
static struct INPUT ground; // dummy


static long fallCount(struct INPUT* remove, int ** arrFallenParts){
    long ret = 0;
    (*arrFallenParts)[remove->index] = 1;
    for(int i = 0; i < remove->supportsSize; i++) {
        struct INPUT* next = remove->supports[i];
        int solidRest = 0;
        for(int k = 0; k < next->restsOnSize; k++) {
            if(next->restsOn[k] == 0) {
                solidRest = 1;
                break;
            }
        }
        if(!solidRest) {
            ret++;
            (*arrFallenParts)[next->index] = 1;
        }
    }
    for(int i = 0; i < remove->supportsSize; i++) {
        struct INPUT* next = remove->supports[i];
        int solidRest = 0;
        for(int k = 0; k < next->restsOnSize; k++) {
            if(next->restsOn[k] == 0) {
                solidRest = 1;
                break;
            }
        }
        if(!solidRest) {
            ret++;
            (*arrFallenParts)[next->index] = 1;
        }
    }
    return ret;
}

void day22() {
    printf("Day 22 (2023)\n");
    read();

    printf("inputsize = %d\n", inputLength);
    for(int i = 0; i < inputLength; i++) {
//        printf("%4d > %d %d %d   %d %d %d\n", i, input[i].s.x,input[i].s.y,input[i].s.z,input[i].e.x, input[i].e.y ,input[i].e.z);
            if(input[i].s.x > input[i].e.x) {
                printf("swapped x\n");
            }
            if(input[i].s.y > input[i].e.y) {
                printf("swapped y\n");
            }
            if(input[i].s.z > input[i].e.z) {
                printf("swapped z\n");
            }
            if(input[i].e.x >= wx) {
                wx = input[i].e.x +1;
            }
            if(input[i].e.y >= wy) {
                wy = input[i].e.y +1;
            }
            if(input[i].e.z >= wz) {
                wz = input[i].e.z +1;
            }
    }
    printf("wx=%d wy=%d wz=%d\n",wx,wy,wz);
    int matrixBytes = sizeof(struct INPUT*) * 20 *20*400;
    matrix = malloc(matrixBytes);
    if(matrix == NULL) {printf("not enough heap available %d\n", matrixBytes);exit(99);}

    for(int i = 0; i < inputLength; i++) {
        struct INPUT* p = &input[i];
        for(int x = p->s.x; x <= p->e.x; x++) {
            for(int y = p->s.y; y <= p->e.y; y++) {
                for(int z = p->s.z; z <= p->e.z; z++) {
                    (*matrix)[x][y][z] = p;
                }
            }
        }
    }
    printf("matrix filled\n");

    for(int layer = 1; layer < wz; layer++) {
        for(int x = wx - 1; x >=0; x--) {
            for(int y = wy - 1; y >=0; y--) {
                struct INPUT* b = (*matrix)[x][y][layer];
                if(b==NULL)continue;
                int newZ = layer-1;
                while(newZ>0) {
                    for(int xN = b->s.x; xN <= b->e.x; xN++) {
                        for(int yN = b->s.y; yN <= b->e.y; yN++) {
                            if((*matrix)[xN][yN][newZ] != NULL) {
                                goto newLayerFound;
                            }
                        }
                    }
                    newZ--;
                }
                newLayerFound:
                newZ++;
                for(int xN = b->s.x; xN <= b->e.x; xN++) {
                    for(int yN = b->s.y; yN <= b->e.y; yN++) {
                        (*matrix)[xN][yN][layer] = NULL;
                        (*matrix)[xN][yN][newZ] = b;
                    }
                }
            }
        }
    }
    printf("gravity switched on. all blocks fallen.\n");


    ground.index = 1999;
    for(int x = wx - 1; x >=0; x--) {
        for(int y = wy - 1; y >=0; y--) {
            struct INPUT* prev = &ground;
            for(int z = 1; z < wz; z++) {
                struct INPUT* b = (*matrix)[x][y][z];
                if(b && prev && b != prev) {
                    for(int k = 0; k < b->restsOnSize; k++) {
                        if(b->restsOn[k] == prev) goto loopBreaked;
                    }
                    b->restsOn[b->restsOnSize++] = prev;
                    loopBreaked:
                    for(int k = 0; k < prev->supportsSize; k++) {
                        if(prev->supports[k] == b) goto loop2Breaked;
                    }
                    prev->supports[prev->supportsSize++] = b;
                    loop2Breaked:
                    ;
                }
                prev = b;
            }
        }
    }
    for(int z = 1; z < wz; z++) {
        int counter = 0;
        char arr[2000] = {0};
        for(int x = wx - 1; x >=0; x--) {
            for(int y = wy - 1; y >=0; y--) {
                struct INPUT* b = (*matrix)[x][y][z];
                if(b==NULL) continue;
                if(arr[b->index] == 0) {
                    arr[b->index] = 1;
                    matrixSum[z][counter++] = b;
                }
            }
        }
        if(counter > 20){
                printf("counter=%d\n", counter);
                exit(20);
        }
    }


    printf("support tree built\n");

    long result = 0;
    for(int i = 0; i < inputLength; i++) {
//        printf("BRICK %d:\n",i);
        int cantRemove = 0;
        for(int j = 0; j < input[i].supportsSize; j++) {
            struct INPUT* supports = input[i].supports[j];
            if(supports->restsOnSize == 1) {
                cantRemove++;
            }
    //        printf("  supports %d which rests on %d boxes\n", supports->index, supports->restsOnSize);

        }
        if(cantRemove == 0) {
            result++;
        }else{
        }
    }
    printf("RESULT1 = %ld\n\n\n", result);

    result = 0;
    for(int i = 0; i < inputLength; i++) {
        long t = 0;
        char fallenBricks[2000] = {0};
        struct INPUT* remove = &input[i];
        fallenBricks[i] = 1;
        printf("INDEX = %d\n",i);
        for(int layer = 1; layer < wz; layer++) {
            int le = 0;
            struct INPUT* check = matrixSum[layer][le++];
            while(check) {
                // check if check falls
//                printf("  checking %d in layer %d\n", check->index, layer);
                int restCount = 0;
                for(int k = 0; k < check->restsOnSize; k++) {
                    struct INPUT* checkRestsOn = check->restsOn[k];
                    if(fallenBricks[checkRestsOn->index] == 0) {
                        restCount++;
                    }
                }
                if(restCount == 0) {
                    if(fallenBricks[check->index] == 0) {
                        fallenBricks[check->index] = 1;
                        t++;
//                        printf(" %d falling\n", check->index);
                    }
                }
                check = matrixSum[layer][le++];
            }
        }

        //t+= fallCount(remove, &fallenBricks);
        if(t!=0){
            printf("removing %d would cause %ld to fall\n", i, t);
        }
        result+= t;
    }
    printf("RESULT2 = %ld\n", result); // 85006


    if(0)
    while(1){
            getchar();
    }

}

static void read() {
    FILE* inputfile = fopen("./input/input22.txt","r");
    int c;
    int counter = 0;
    do
    {
        c = (char)fgetc(inputfile);
        if(c == '\n') {
            input[inputLength].index = inputLength;
            inputLength++;
            counter= 0;
        } else if(c >= '0' && c<= '9') {
            int* number;
            switch(counter) {
                case 0: number = &input[inputLength].s.x;break;
                case 1: number = &input[inputLength].s.y;break;
                case 2: number = &input[inputLength].s.z;break;
                case 3: number = &input[inputLength].e.x;break;
                case 4: number = &input[inputLength].e.y;break;
                case 5: number = &input[inputLength].e.z;break;
            }
            *number *=10;
            *number += c - '0';
        }else{
            counter++;
        }
    } while(c != EOF);
    fclose(inputfile);
}
