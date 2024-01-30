
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void read();

static int wi = 0;
static int wj = 0;
static int matrix[20000][20000] = {0}; // can i a few more terrabytes of ram for part 2 pls?

static struct INPUT{
    int length;
    int direction; //0 up, 1 right

    long hexL;
    int hexD;
};

static struct INPUT input[1000];
static int inputLength = 0;



//=========================== QEUEUEUEUEUE ========================

static struct QCONTENT{
    long long i;
    long long j; // corners of the drawn figure
    struct QCONTENT* prev;
    struct QCONTENT* next;
    long index;
};


static struct QUEUEITEM{
    struct QUEUEITEM* next;
    struct QUEUEITEM* prev;
    struct QCONTENT* item;
};

static struct QUEUE{
    struct QUEUEITEM* start;
    long length;
};


static void q_insert(struct QUEUE* queue, struct QCONTENT* content) {
    queue->length ++;
     struct QUEUEITEM* s = queue->start;
     long long cost = content->i;
     struct QUEUEITEM* q = malloc(sizeof(struct QUEUEITEM));
     q->item = content;

     if(s==NULL) {
        queue->start = q;
        q->prev = NULL;
        q->next = NULL;
        return;
     }

     while(1) {
        if(s->item->i >= cost){
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

static struct QCONTENT* q_pop(struct QUEUE* queue) {
    queue->length--;
    struct QUEUEITEM* q = queue->start;
    struct QCONTENT* first = q->item;
    queue->start = q->next;
    if(queue->start != NULL)
        queue->start->prev = NULL;
    free(q);
    return first;
}

static void q_remove(struct QUEUE* queue, struct QCONTENT* item) {
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

static inline struct QCONTENT* q_peek(struct QUEUE* queue) {
    if(queue->start == NULL) return NULL;
    return queue->start->item;

}


//=========================== QEUEUEUEUEUE ========================

//=== part 2

static struct QUEUE corners;

static struct QCONTENT* newC(long long i , long long j, int index, struct QCONTENT* prev, struct QCONTENT* next) {
    struct QCONTENT* ret = malloc(sizeof(struct QCONTENT));
    ret->i = i;
    ret->j = j;
    ret->index = index;
    ret-> prev = prev;
    ret-> next = next;
    return ret;
}

static void calcPath() {
    long long i = 10000000;
    long long j = 10000000;
    int index = 0;
    struct QCONTENT* start = newC(i,j, index++, NULL, NULL);
//    q_insert(&corners, start);

    struct QCONTENT* prev = start;

    for(int o = 0; o < inputLength; o++) {
        int dir = input[o].hexD;
        long le = input[o].hexL;
        switch(dir) {
            case 0: i-=le; break;
            case 2: i+=le; break;
            case 1: j+=le; break;
            case 3: j-=le; break;
        }
        struct QCONTENT* n = newC(i,j, index++, prev, NULL);
        q_insert(&corners, n);
        prev->next = n;
        prev = n;
    }
    if(start->i != prev->i || start->j != prev->j) {
//      printf("start and end not THE SAME: %lld %lld %lld %lld", start->i,start->j, prev->i, prev->j);
        exit(77);
    }
    // fix loop
    prev->next = start->next;
    start->next->prev = prev;
    free(start);

    struct QUEUEITEM* a = corners.start;
    while(a != NULL) {
//        printf("%d. next %d. prev %d.   %lld %lld\n", a->item->index, a->item->next->index, a->item->prev->index, a->item->i, a->item->j);
        a = a->next;
    }


}


static int compar(const void * a, const void* b){
    const struct QCONTENT** aa = a;
    const struct QCONTENT** bb = b;

    return (*aa)-> j - (*bb)->j;
}

static long long calcAreaFromCorners() {
    long long area = 0;
    struct QCONTENT* activeCorners[1000];
    int activeCornersCount = 0;
    long long activeSpan = 0;
    long long lastI = 0; // doesnt matter if activeSpan = 0


    // example.  xx = area
    // xxxx      xx  xxxx
    // xxxx  xx  xx  xxxxx
    //  xxx  xx      xxxxx
    //  xxx  xx      xxxxx


    while(corners.length != 0) {
        long long nextI =q_peek(&corners)-> i;
 //       printf("\nNEW CYCLE with %d old corners at i=%lld\n", activeCornersCount, nextI);

        struct QCONTENT* activeCornersWorking[1000];
        int activeCornersWorkingCount = 0;
        while(1) {
            struct QCONTENT* q = q_peek(&corners);
            if(!q) break;
            if(q->i != nextI) break;
            activeCornersWorking[activeCornersWorkingCount++] = q_pop(&corners);
 //           printf("NEW CORNERS FOR THIS CYCLE: %d\n", activeCornersWorking[activeCornersWorkingCount-1] ->index);
        }
        qsort(activeCornersWorking,activeCornersWorkingCount,sizeof(struct QCONTENT*),compar );

        for(int p = 0; p < activeCornersWorkingCount; p++) {
 //           printf(" workingItem %d\n", activeCornersWorking[p]->index);
        }

        // calculate are in this line:
        long long lineArea = 0;
        {
            if(activeCornersCount % 2 == 1 ||activeCornersWorkingCount % 2 == 1) {
                printf("ERROR %d %d", activeCornersCount, activeCornersWorkingCount);
                exit(11);
            }
            int i = 0;
            int p=0;
            long lastLargest = -9999999999999;
            while(1) {
                if(i>=activeCornersCount) break;
                if(p>=activeCornersWorkingCount) break;
                struct QCONTENT * aa = activeCorners[i];
                struct QCONTENT * ab = activeCorners[i+1];
                struct QCONTENT * ba = activeCornersWorking[p];
                struct QCONTENT * bb = activeCornersWorking[p+1];

                if(aa->j < lastLargest || ba->j < lastLargest) {
                    exit(44);
                }

                if(aa->j >= ba->j && ab->j <= bb->j) {
                    i+=2;
                    continue;
                }
                if(ba->j >= aa->j && bb->j <= ab->j) {
                    p+=2;
                    continue;
                }

                if(ab->j < ba->j) {
                    lineArea += ab->j - aa->j + (aa->j == lastLargest ? 0 : 1);
                    i+=2;
                    continue;
                }
                if(bb->j < aa->j) {
                    lineArea += bb->j - ba->j + (ba->j == lastLargest ? 0 : 1);
                    p+=2;
                    continue;
                }
                long long smallest = aa->j;
                long long largest = ab->j;
                if(largest < bb->j) {
                    largest = bb->j;
                }
                if(smallest > ba->j) {
                    smallest = ba->j;
                }
                i+=2;
                p+=2;
                lineArea += largest - smallest + (smallest == lastLargest ? 0 : 1);
                // JUST A TEST
                lastLargest = largest;
            }
            for(i;i < activeCornersCount; i+=2) {
                if(activeCorners[i]->j < lastLargest) exit(47);
                lineArea+=(activeCorners[i+1] -> j) -(activeCorners[i]->j) + ((activeCorners[i]->j == lastLargest)? 0 : 1);
            }
            for(p;p < activeCornersWorkingCount; p+=2) {
                if(activeCornersWorking[p]->j < lastLargest) exit(46);
                lineArea+=(activeCornersWorking[p+1] -> j) -(activeCornersWorking[p]->j)+ ((activeCornersWorking[p]->j == lastLargest)? 0 : 1);
            }
            // done calculating line area. now we need to get the new active corners.
        }


        // remove all new points that match points in the previous array. they are lines connection to above.
        for(int i = 0; i < activeCornersCount; i++) {
            struct QCONTENT * a = activeCorners[i];
//            printf("checking old corner %d\n",a->index);
            for(int p = 0; p < activeCornersWorkingCount; p++) {
                if(activeCornersWorking[p] -> j == a-> j) {
                    // vertical line between old and new i detected
                    struct QCONTENT* replaceByThisCorner = NULL;
                    if(activeCornersWorking[p]->next->i == nextI) {
                        if(activeCornersWorking[p]->next->next->i < nextI) {
//                            printf("n requesting removal: %d(%lld) -> %d(%lld) -> %d(%lld)\n", activeCornersWorking[p]->index,activeCornersWorking[p]->j, activeCornersWorking[p]->next->index, activeCornersWorking[p]->next->j, activeCornersWorking[p]->next->next->index, activeCornersWorking[p]->next->next->j);
                            goto removeWithoutReplacement;
                        }
                        replaceByThisCorner = activeCornersWorking[p]->next;
                        // the line is actually still there. just shifted. replace:
                    }else if(activeCornersWorking[p]->prev->i == nextI) {
                        if(activeCornersWorking[p]->prev->prev->i < nextI) {
                            //long diff =activeCornersWorking[p]->j - activeCornersWorking[p]->prev->j;
                            //if(diff < 0 ) diff = -diff;
                            //reMovedAreaInThisLine +=diff;
 //                           printf("p requesting removal: %d(%lld,%lld) -> %d(%lld,%lld) -> %d(%lld,%lld)\n", activeCornersWorking[p]->index,activeCornersWorking[p]->i,activeCornersWorking[p]->j, activeCornersWorking[p]->prev->index, activeCornersWorking[p]->prev->i,activeCornersWorking[p]->prev->j, activeCornersWorking[p]->prev->prev->index,activeCornersWorking[p]->prev->prev->i, activeCornersWorking[p]->prev->prev->j);

                            goto removeWithoutReplacement;
                        }

                        replaceByThisCorner = activeCornersWorking[p]->prev;
                        // the line is actually still there. just shifted. replace:
                    }else{
                        printf("SHOULD NEVER HAPPEN\n");
                    }
                    if(replaceByThisCorner) {
//                        printf("corner shifted to %d over %d\n", replaceByThisCorner->index, activeCornersWorking[p]->index);
                        //long areaDiff = replaceByThisCorner->j - a->j;
                        //if(i%2) {
                        //    reMovedAreaInThisLine -= areaDiff;
                        //}else{
                        //    reMovedAreaInThisLine += areaDiff;
                        //}
                        activeCorners[i] = replaceByThisCorner;
                        int move = 0;
                        for(int k = 0; k < activeCornersWorkingCount; k++) {
                            if(move == 0) {
                                if(activeCornersWorking[k] == replaceByThisCorner) {
                                    move = 1;
                                    if(p > k){
                                        p--; // correct p so later when removing the p element the correct one is removed
                                    }
                                }
                            }else{
                                activeCornersWorking[k-1] =activeCornersWorking[k];
                            }
                        }
                        activeCornersWorkingCount--;
                    }
                    if(0) {
                        removeWithoutReplacement:
//                        printf("removing %d\n", activeCorners[i]->index);
                        for(int k = i+1; k < activeCornersCount; k++) {
                            activeCorners[k-1] = activeCorners[k];
                        }
                        activeCornersCount--;
                        i--; // correct i since we move a new element on this index
                    }
 //                   printf("removing matched new working corner %d\n", activeCornersWorking[p]->index);
                    // remove from nextI working list, since this point is just a line to a point above.
                    for(int k = p+1; k < activeCornersWorkingCount; k++) {
                        activeCornersWorking[k-1] = activeCornersWorking[k];
                    }
                    activeCornersWorkingCount--;
                    break;
                }
            }
        }
        // now all old points are removed from activeCorners and some shifted lines are already inserted. Maybe there are also additional new areas:
        long long newIntroducedArea = 0;
        for(int k = 1; k < activeCornersWorkingCount; k+=2) {
 //           printf("new area points: %d %d\n", activeCornersWorking[k-1] ->index, activeCornersWorking[k] ->index);
            newIntroducedArea += activeCornersWorking[k]->j - activeCornersWorking[k-1]->j + 1;
            activeCorners[activeCornersCount++] = activeCornersWorking[k-1];
            activeCorners[activeCornersCount++] = activeCornersWorking[k];
        }
        qsort(activeCorners,activeCornersCount,sizeof(struct QCONTENT*),compar );
        area += (nextI - lastI - 1)*activeSpan;
        area += lineArea;

//        printf("CYCLE DONE added area = (%lld-%lld-1)*%lld=%lld       and  additionally %lld   ->SUM=%lld\n", nextI, lastI,activeSpan, (nextI - lastI-1)*activeSpan, lineArea, area);
        activeSpan = 0;
        for(int i = 1; i < activeCornersCount; i+=2) {
            activeSpan += activeCorners[i]->j - activeCorners[i-1]->j + 1;
        }
        lastI = nextI;
    }

    return area; // 58287674283787  8603951630450607539   54058827307667 to high  54058824661865  54058824661845

}

//======

static void markPath() {
    int i = 500;
    int j = 500;
    matrix[i][j] = '#';
    for(int o = 0; o < inputLength; o++) {
        int dir = input[o].direction;
        int l = input[o].length;
        switch(dir) {
        case 0:
            for(int k = 0; k < l; k++) {
                i--;
                matrix[i][j] = '#';
            }
            break;
        case 2:
            for(int k = 0; k < l; k++) {
                i++;
                matrix[i][j] = '#';
            }
            break;
        case 3:
            for(int k = 0; k < l; k++) {
                j--;
                matrix[i][j] = '#';
            }
            break;
        case 1:
            for(int k = 0; k < l; k++) {
                j++;
                matrix[i][j] = '#';
            }
            break;
        }
        if(i >= wi) {
            wi = i + 1;
        }
        if(j >= wj) {
            wj = j + 1;
        }
        if(i < 10 ||  j< 10) {
            printf("MATRIX MIGHT BE TO SMALL OR LEFT UP: %d %d\n" ,i,j);
        }
    }

}

static void printM() {
    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
            if(matrix[i][j]) {
                printf("%c", matrix[i][j]);
            }else{
                printf(" ");

            }
        }
                printf("\n");


        }

}

static long calcArea3() {
    long area = 0;
    int symbol = '`';
    for(int i = 0; i < wi; i++) {
        if(matrix[i][0] == 0) {
            matrix[i][0] = symbol;
        }
        if(matrix[i][wj-1] == 0) {
            matrix[i][wj-1] = symbol;
        }
    }
    for(int j = 0; j < wj; j++) {
        if(matrix[0][j] == 0) {
            matrix[0][j] = symbol;
        }
        if(matrix[wi - 1][j] == 0) {
            matrix[wi - 1][j] = symbol;
        }
    }

    int found = 0;
    do{
        found = 0;
        for(int i = 0; i < wi; i++) {
            for(int j = 0; j < wj; j++) {
                if(matrix[i][j] == symbol) {
                    if(i > 0) {
                        if(matrix[i-1][j] == 0) {
                            matrix[i-1][j] = symbol;
                            found++;
                        }
                    }
                    if(j > 0) {
                        if(matrix[i][j-1] == 0) {
                            matrix[i][j-1] = symbol;
                            found++;
                        }
                    }

                    if(j < wj-1) {
                        if(matrix[i][j+1] == 0) {
                            matrix[i][j+1] = symbol;
                            found++;
                        }
                    }
                    if(i < wi - 1) {
                        if(matrix[i+1][j] == 0) {
                            matrix[i+1][j] = symbol;
                            found++;
                        }
                    }
                }
            }
        }
    }while(found != 0);


    for(int i = 0; i < wi; i++) {
        for(int j = 0; j < wj; j++) {
                if(matrix[i][j] == 0 || matrix[i][j] == '#') area++;
        }
    }
    return area;


}


void day18(int part) {
    printf("Day 18 (2023)\n");
    read();

    for(int i = 0; i < inputLength; i++) {
 //       printf(" %d   %d   %d   hex: %ld %d \n", i, input[i].direction, input[i].length, input[i].hexL, input[i].hexD);
    }

    if(part == 1) {
        markPath();
        printf("GRIDSIZE = %d %d   instructinos = %d\n", wi , wj, inputLength);
        long area = calcArea3();
        //printM();
        printf("AREA %d\n",area ); // 22753 // 24289 //1994 // 5421 // 30463. two digits...
    }else{
        calcPath();
        long long p2 = calcAreaFromCorners();
        printf("p2 result = %lld\n", p2);
    }




/*
    while(1){
            getchar();
    }
*/
}

static void read() {
    FILE* inputfile = fopen("./input/input18.txt","r");
    int c;
    int firstNumber = -1;
    int secondNumber = 0;
    int thirdNumberIndex = 0;
    int secondNumberDone = 0;
    char buffer[ 10];
    do
    {
        c = (char)fgetc(inputfile);
        if(c == '\n') {
            input[inputLength].direction = firstNumber;
            switch(firstNumber){
                case 'U': input[inputLength].direction = 0; break;
                case 'R': input[inputLength].direction = 1; break;
                case 'D': input[inputLength].direction = 2; break;
                case 'L': input[inputLength].direction = 3; break;
            }

            input[inputLength].length = secondNumber;
            input[inputLength].hexL = 0;
            for(int i = 0; i < 5; i++) {
                input[inputLength].hexL *= 16;
                if(buffer[i] >= '0' &&  buffer[i] <= '9' ) {
                    input[inputLength].hexL +=buffer[i] - '0';
                }else{
                    switch(buffer[i]) {
                        case 'a':input[inputLength].hexL += 10; break;
                        case 'b':input[inputLength].hexL += 11; break;
                        case 'c':input[inputLength].hexL += 12; break;
                        case 'd':input[inputLength].hexL += 13; break;
                        case 'e':input[inputLength].hexL += 14; break;
                        case 'f':input[inputLength].hexL += 15; break;
                    }
                }
            }
            switch(buffer[5]){
                case '0': input[inputLength].hexD  = 1; break;
                case '1': input[inputLength].hexD  = 2; break;
                case '2': input[inputLength].hexD  = 3; break;
                case '3': input[inputLength].hexD  = 0; break;
            }

            inputLength++;
            firstNumber = -1;
            secondNumber = 0;
            thirdNumberIndex = 0;
            secondNumberDone = 0;
        }else{
            if(firstNumber == -1 ){
                if(c >= 'A' && c<= 'Z') {
                    firstNumber = c;
                }
            }
            else if(secondNumberDone == 0 && c != '(') {
                if(c >= '0' && c<= '9') {
                    secondNumber *= 10; // two digits possible....
                    secondNumber += c - '0';
                }
            }else if(c == '(') {
                secondNumberDone = 1;
            }else if(c != ' ' && c != '(' && c != ')'&& c != '#') {
                buffer[thirdNumberIndex] = c;
                thirdNumberIndex++;
            }
        }
    } while(c != EOF);
    fclose(inputfile);
}
