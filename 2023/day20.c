
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//=========================== QEUEUEUEUEUE ========================

static struct QCONTENT{
    struct MODULE* target;
    struct MODULE* source;
    char isHigh;
};


static struct STACK{
    #define SSIZE 1000
    struct QCONTENT* ringB[SSIZE];
    int firstIndex;
    int nextIndex;
};

static void stack_insert(struct STACK* stack, struct QCONTENT* item) {
    stack->ringB[stack->nextIndex] = item;
    stack->nextIndex++;
    if(stack->nextIndex == SSIZE) {
        stack->nextIndex = 0;
    }
};

static struct QCONTENT* stack_remove(struct STACK* stack) {
    if(stack->nextIndex == stack->firstIndex) {
        return NULL;
    }
    struct QCONTENT* ret =  stack->ringB[stack->firstIndex];
    stack->firstIndex++;
    if(stack->firstIndex == SSIZE) {
        stack->firstIndex = 0;
    }
    return ret;
}


//=========================== QEUEUEUEUEUE ========================


static struct MODULE {
    int index;
    char type;
    char name[3];
    char outputs[10][3];
    struct MODULE* out[10];
    struct MODULE* in[30];
    int intputCount;
    int outputCount;

    char flipFlopMemory;
    char conjunctionMemory[30];
};


static struct MODULE modules[100] = {0};
static int moduleCount = 0;
static struct MODULE* start;
static struct STACK stack;

static void read();

static struct QCONTENT* createNewContent(struct QCONTENT* source, struct QCONTENT* next, char isHigh) {
    struct QCONTENT* ret = malloc(sizeof(struct QCONTENT));
    ret->isHigh = isHigh;
    ret->target = next;
    ret->source = source;

    return ret;
}


int day20(int part2) {
    printf("Day 20 (2023)\n");
    read();
    printf("file read %d\n", moduleCount);


    for(int i = 0; i < moduleCount; i++) {
        if(modules[i].type == 'b') {
            start = &modules[i];
        }
        for(int j = 0; j < modules[i].outputCount; j++) {
            int found = 0;
            for(int k = 0; k < moduleCount; k++) {
                if(strcmp(modules[k].name,modules[i].outputs[j])== 0) {
                    modules[i].out[j] = &modules[k];
                    modules[k].in[modules[k].intputCount++] = &modules[i];

                    found = 1;
                    break;
                }
            }
        }
    }

    if(0){
        for(int i = 0; i < moduleCount; i++) {
            printf("%d %c   %s:\n",modules[i].index,modules[i].type, modules[i].name);
            for(int j = 0; j < modules[i].outputCount; j++) {
                if(modules[i].out[j] != NULL)
                printf("    > %s\n", modules[i].out[j]->name);
            }
        }
        printf("=====\n");
    }

    long long highCount = 0;
    long long lowCount = 0;

    for(int u = 0; (u < 1000 || part2 == 1); u++) {
        stack_insert(&stack, createNewContent(NULL, start,0));
        if(u%10000==0){
            printf("DONE %d pulses\n", u);
        }

        while(stack.firstIndex != stack.nextIndex) {
            struct QCONTENT* nextItem = stack_remove(&stack);
            struct MODULE* module = nextItem->target;
            if(nextItem->isHigh) {
                highCount++;
            }else{
                lowCount++;
            }
            if(module==NULL){
                if(nextItem->isHigh == 0) {
                    printf("PART 2 RESULT = %d\n", u);
                    exit(0);
                }
                free(nextItem);
                continue;
            }
            if(module->type == 'b') {
                for(int i = 0; i < module->outputCount; i++) {
                    stack_insert(&stack, createNewContent(module, module->out[i],nextItem->isHigh));
                }
            }
            if(module->type == '%') {
                if(nextItem->isHigh == 0) {
                    module->flipFlopMemory = !module->flipFlopMemory;
                    for(int i = 0; i < module->outputCount; i++) {
                        stack_insert(&stack, createNewContent(module, module->out[i],module->flipFlopMemory));
                    }
                }
            }
            if(module->type == '&') {
                int allHigh = 1;
                for(int i = 0; i < module->intputCount; i++) {
                    if(module->in[i] == nextItem->source) {
                        module->conjunctionMemory[i] = nextItem->isHigh;
                    }
                    if(module->conjunctionMemory[i] == 0) {
                        allHigh = 0;
                    }
                }
                for(int i = 0; i < module->outputCount; i++) {
                    stack_insert(&stack, createNewContent(module, module->out[i],!allHigh));
                }
            }
            free(nextItem);
        }
    }

    printf("RESULT= %lld * %lld = %lld\n", highCount, lowCount, highCount * lowCount);

//    while(1){
//            getchar();
//    }
    return 0;
}

static void read() {
    FILE* inputfile = fopen("./input/input20.txt","r");

    int bufferLength = 255;
    char buffer[bufferLength];

    while(fgets(buffer, bufferLength, inputfile)) {
        int index = 0;
        modules[moduleCount].type = buffer[0];
        if(buffer[index] == 'b') {
            index+=15;
            modules[moduleCount].name[0] = 0;
        }else{
            index++;
            modules[moduleCount].name[0] = buffer[index++];
            modules[moduleCount].name[1] = buffer[index];
            modules[moduleCount].name[3] = '\0';
            index +=5;
        }
        modules[moduleCount].flipFlopMemory = 0;
        modules[moduleCount].outputCount = 0;
        modules[moduleCount].index = moduleCount;
        while(buffer[index] != '\0' && buffer[index] != '\n'){
            if(buffer[index] == ' '){
                index++;
                continue;
            }
            if(buffer[index] == ','){
                index++;
                continue;
            }
            modules[moduleCount].outputs[modules[moduleCount].outputCount][0] = buffer[index++];
            modules[moduleCount].outputs[modules[moduleCount].outputCount][1] = buffer[index++];
            modules[moduleCount].outputs[modules[moduleCount].outputCount][2] = '\0';
            modules[moduleCount].outputCount++;
        }

        moduleCount ++;
    }


    fclose(inputfile);
}
