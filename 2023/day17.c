
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void read();

static int wi = 0;
static int wj = 0;
static int matrix[200][200];

static struct SEARCHHEAD{
    int i;
    int j;
    int lastDir; // 0 1 2 3
    int stepsWithSameDirection;
    int pathLen;
    int approximatedOveralCost;
};


//=========================== QEUEUEUEUEUE ========================




static struct QUEUEITEM{
    struct QUEUEITEM* next;
    struct QUEUEITEM* prev;
    struct SEARCHHEAD* item;
};

static struct QUEUE{
    struct QUEUEITEM* start;
    long length;
};


static void q_insert(struct QUEUE* queue, struct SEARCHHEAD* head) {
    queue->length ++;
     struct QUEUEITEM* s = queue->start;
     int cost = head->approximatedOveralCost;
     struct QUEUEITEM* q = malloc(sizeof(struct QUEUEITEM));
     q->item = head;

     if(s==NULL) {
        queue->start = q;
        q->prev = NULL;
        q->next = NULL;
        return;
     }

     while(1) {
        if(s->item->approximatedOveralCost >= cost){
            // insert before s
            q->next = s;
            q->prev = s->prev;
            if(s->prev == NULL) {
                queue->start = q;
            }else{
                s->prev->next = q;
            }
            s->prev = q;
            break;
        }
        struct QUEUEITEM* n =  s->next;
        if(n == NULL) {
            s->next = q;
            q->next = NULL;
            q->prev = s;
            break;
        }
        s = n;
     }
}

static struct SEARCHHEAD* q_pop(struct QUEUE* queue) {
    queue->length--;
    struct QUEUEITEM* q = queue->start;
    struct SEARCHHEAD* first = q->item;
    queue->start = q->next;
    if(queue->start != NULL)
        queue->start->prev = NULL;
    free(q);
    return first;
}

static void q_remove(struct QUEUE* queue, struct SEARCHHEAD* item) {
    struct QUEUEITEM* s = queue->start;

    if(s->item == item) {
        queue->start = s->next;
        if(s->next != NULL) {
            s->next->prev = NULL;
        }
        free(s);
        return;
    }

    while(1) {
        if(s==NULL){
            break;
        }
        if(s ->item == item) {
            if(s->next != NULL){
                s->next->prev = s->prev;
            }
            if(s->prev != NULL) {
                s->prev->next = s->next;
            }
            free(s);
            break;
        }
        s = s->next;
    }
}

static inline struct SEARCHHEAD* q_peek(struct QUEUE* queue) {
    if(queue->start == NULL) return NULL;
    return queue->start->item;

}


//=========================== QEUEUEUEUEUE ========================

static struct QUEUE queueHead = {0};
static struct QUEUE queuesPerMatrixEntry[200][200] = {0};
static int lowestNumbers[5000];



static inline void calcAproxCost(struct SEARCHHEAD* h) {
    h->approximatedOveralCost = h->pathLen +  wi +  wj -  h->i -  h->j ; // eh. Is this of by 2?
    //h->approximatedOveralCost = h->pathLen + lowestNumbers[wi +  wj -  h->i -  h->j - 3] ;

}

static struct SEARCHHEAD* createNewSearchhead(int i, int j, int lastDir, int stepsWithSameDir, int pathLen) {
    struct SEARCHHEAD* queueHead = malloc(sizeof(struct SEARCHHEAD));
    queueHead->i = i;
    queueHead->j = j;
    queueHead->lastDir = lastDir;
    queueHead->stepsWithSameDirection = stepsWithSameDir;
    queueHead->pathLen = pathLen;
    calcAproxCost(queueHead);
    return queueHead;
}




static int shouldInsert1(struct SEARCHHEAD* h) {
    struct QUEUEITEM* s = queuesPerMatrixEntry[h->i][h->j].start;

    while(1) {
        if(s==NULL){
            break;
        }
        struct SEARCHHEAD* q = s->item;
        if(q->lastDir == h->lastDir){
            if(q->stepsWithSameDirection <= h->stepsWithSameDirection) {
                if(q->pathLen <= h->pathLen) {
                    //printf("REDUCING LOAD\n");
                    return 0;
                }
            }
        }


        s = s->next;
    }

    return 1;
}

static int shouldInsert2(struct SEARCHHEAD* h) {
    struct QUEUEITEM* s = queuesPerMatrixEntry[h->i][h->j].start;

    while(1) {
        if(s==NULL){
            break;
        }
        struct SEARCHHEAD* q = s->item;
        if(q->lastDir == h->lastDir){
            if(q->stepsWithSameDirection == h->stepsWithSameDirection) {
                if(q->pathLen <= h->pathLen) {
                    return 0;
                }
            }
        }


        s = s->next;
    }

    return 1;
}

static void printQ() {
    struct QUEUEITEM* s = queueHead.start;
    while(1) {
        if(s==NULL){
            printf("---------\n");
            break;
        }
        printf(" > [%d][%d] with len=%d dir=%d for %d   approx=%d\n", s->item->i, s->item->j, s->item->pathLen, s->item->lastDir, s->item->stepsWithSameDirection, s->item->approximatedOveralCost);

        s = s->next;
    }
}

static int calcNext(int direction, int i, int j, int* newi, int* newj) {
    switch(direction) {
    case 0:
        if(i <= 0) {
            return 0;
        }else{
            *newi = i-1;
            *newj = j;
            return 1;
        }
    case 2:
        if(i >= wi -1) {
            return 0;
        }else{
            *newi = i+1;
            *newj = j;
            return 1;
        }
    case 3:
        if(j <= 0) {
            return 0;
        }else{
            *newi = i;
            *newj = j-1;
            return 1;
        }
    case 1:
        if(j >= wj -1) {
            return 0;
        }else{
            *newi = i;
            *newj = j+1;
            return 1;
        }
    }
    printf("FATAL ERROR\n\n\n");
    return 1;
}


static int calcNext2(int direction, int requiredSpace, int i, int j, int* newi, int* newj) {
    switch(direction) {
    case 0:
        if(i < requiredSpace) {
            return 0;
        }else{
            *newi = i-1;
            *newj = j;
            return 1;
        }
    case 2:
        if(wi - i <= requiredSpace) {
            return 0;
        }else{
            *newi = i+1;
            *newj = j;
            return 1;
        }
    case 3:
        if(j < requiredSpace) {
            return 0;
        }else{
            *newi = i;
            *newj = j-1;
            return 1;
        }
    case 1:
        if(wj - j <= requiredSpace) {
            return 0;
        }else{
            *newi = i;
            *newj = j+1;
            return 1;
        }
    }
    printf("FATAL ERROR\n\n\n");
    return 1;
}

static int printCounter = 0;


static struct SEARCHHEAD* solve1() {
    while(1) {
        //printQ();

        int lowest = 9999999;
        struct SEARCHHEAD* s;
        //s = q_pop(&queueHead);

        for(int i = wi - 1 ; i >= 0; i--) { // new stratgy: dont't maintain a gigantic list
            for(int j = wj - 1; j >= 0; j--) {
                struct SEARCHHEAD* peeked = q_peek(&queuesPerMatrixEntry[i][j]);
                if(peeked != NULL) {
                    if(peeked->approximatedOveralCost < lowest){
                        lowest = peeked->approximatedOveralCost;
                        s = peeked;
                    }
                }
            }
        }
        q_remove(&queuesPerMatrixEntry[s->i][s->j], s);

        //if(queueHead.length % 1000 == 0){
        if(s->pathLen > printCounter){
            printCounter++;
            printf("[%5d] CHECKING NEXT ITEM [%d][%d] with len=%d dir=%d for %d   approx=%d\n",queueHead.length, s->i, s->j, s->pathLen, s->lastDir, s->stepsWithSameDirection, s->approximatedOveralCost);
        }
        if((s->i ==( wi - 1 ))&& (s->j == (wj - 1))) {
            return s;
        }

        // TURN LEFT AND RIGHT:
        for(int d = 1; d < 4; d+=2) {
            int newD = (s->lastDir + d) % 4;
            int newI;
            int newJ;
            if(calcNext(newD, s->i, s->j, &(newI), &(newJ))) {
                struct SEARCHHEAD * n = createNewSearchhead(newI, newJ, newD, 1, s->pathLen + matrix[newI][newJ]);
                if(shouldInsert1(n)){
                    //q_insert(&queueHead, n);
                    q_insert(&queuesPerMatrixEntry[newI][newJ], n);
                }else{
                    free(n);
                }
            }
        }

        // GO STRAIGHT
        if(s->stepsWithSameDirection < 3) {
            s->stepsWithSameDirection++;
            if(calcNext(s->lastDir, s->i, s->j, &(s->i), &(s->j))) {
                s->pathLen += matrix[s->i][s->j];
                calcAproxCost(s);
                if(shouldInsert1(s)){
                    //q_insert(&queueHead,s);
                    q_insert(&queuesPerMatrixEntry[s->i][s->j], s);
                }else {
                    free(s);
                }
            }else{
                free(s);
            }
        }else{
            free(s);
        }
    }
}

static int compar(const void * a, const void* b){
    return ((int*) a) - ((int*) b);
}

static struct SEARCHHEAD* solve2() {
    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
            lowestNumbers[i*wj + j] = matrix[i][j];
        }
    }
    qsort(lowestNumbers, wi*wj,sizeof(int),compar );
    int sum = 0;
    for(int i = 0; i < wi*wj; i++) {
        sum += lowestNumbers[i];
        lowestNumbers[i] = sum;
    }


    while(1) {
        //printQ();

        int lowest = 9999999;
        int debugL = 0;
        struct SEARCHHEAD* s = NULL;
        //s = q_pop(&queueHead);

        for(int i = wi - 1 ; i >= 0; i--) { // new stratgy: dont't maintain a gigantic list
            for(int j = wj - 1; j >= 0; j--) {
                struct SEARCHHEAD* peeked = q_peek(&queuesPerMatrixEntry[i][j]);
                if(peeked != NULL) {
                    if(peeked->approximatedOveralCost < lowest){
                        lowest = peeked->approximatedOveralCost;
                        debugL = queuesPerMatrixEntry[i][j].length;
                        s = peeked;
                    }
                }
            }
        }
        if(s== NULL) {
            printf(" NO SOLUTION!!!");
            exit(0);
        }
        q_remove(&queuesPerMatrixEntry[s->i][s->j], s);


        if(s->pathLen > printCounter){
            printCounter++;
            printf("[%5d] CHECKING NEXT ITEM [%3d][%3d] with len=%5d dir=%d for %d   approx=%d\n",debugL, s->i, s->j, s->pathLen, s->lastDir, s->stepsWithSameDirection, s->approximatedOveralCost);
        }
        if((s->i ==( wi - 1 ))&& (s->j == (wj - 1))) {
            return s;
        }

        // TURN LEFT AND RIGHT:
        if(s->stepsWithSameDirection >= 4) {
            for(int d = 1; d < 4; d+=2) {
                int newD = (s->lastDir + d) % 4;
                int newI;
                int newJ;
                if(calcNext2(newD, 4, s->i, s->j, &(newI), &(newJ))) {
                    struct SEARCHHEAD * n = createNewSearchhead(newI, newJ, newD, 1, s->pathLen + matrix[newI][newJ]);
                    if(shouldInsert2(n)){
                        //printf("inserting [%d][%d]\n",newI, newJ );
                        //q_insert(&queueHead, n);
                        q_insert(&queuesPerMatrixEntry[newI][newJ], n);
                    }else{
                        free(n);
                    }
                }
            }
        }


        // GO STRAIGHT:
        if(s->stepsWithSameDirection < 10) {
            s->stepsWithSameDirection++;
            int requiredSpace = 4 - s->stepsWithSameDirection;
            if(requiredSpace <= 0) requiredSpace = 1;
            if(calcNext2(s->lastDir, requiredSpace ,s->i, s->j, &(s->i), &(s->j))) {
                s->pathLen += matrix[s->i][s->j];
                calcAproxCost(s);
                if(shouldInsert2(s)){
                        //printf("inserting [%d][%d]\n",s->i, s->j );
                    //q_insert(&queueHead,s);
                    q_insert(&queuesPerMatrixEntry[s->i][s->j], s);
                }else {
                    free(s);
                }
            }else{
                free(s);
            }
        }else{
            free(s);
        }
    }
}


void day17(int part) {
    printf("Day 17 (2023)\n");
    read();
    printf("GRID SIZE = %d %d\n", wi, wj);
    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {

        }
    }




    struct SEARCHHEAD* searchStart1 = createNewSearchhead(0,0,1,0,0);
    q_insert(&queueHead, searchStart1);
    q_insert(&queuesPerMatrixEntry[0][0], searchStart1);
    printQ();

    if(part == 1) {
        struct SEARCHHEAD* solution = solve1();
        printf("RESULT part 1= %d\n",solution->pathLen);
    }else{
        struct SEARCHHEAD* searchStart2 = createNewSearchhead(0,0,2,0,0);
        q_insert(&queueHead, searchStart2);
        q_insert(&queuesPerMatrixEntry[0][0], searchStart2);
        struct SEARCHHEAD* solution = solve2();
        printf("RESULT part2 = %d\n",solution->pathLen);

    }

    while(1){
            getchar();
    }

}

static void read() {
    FILE* inputfile = fopen("./input/input17.txt","r");
    int c;
    int i = 0;
    int j = 0;
    do
    {
        c = (char)fgetc(inputfile);
        matrix[i][j] = c - '0';
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
