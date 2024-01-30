
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
static char matrix[1010][100];
static int rows = 0;

static void read();
static int recursion(const char* buffer, int bufferL, int* numbers, int numbersL, char* previousPath);


struct net{
    int adjMatrix[100][100][100]; // [layer][fromNumber][toNumber]
    int layers ; //= bearbeitete blocks
    int numbers[100];  //[0] means startstete. all other numbers are the next states
    int numberslen; // excluding te´he +1
    char str[300];
    int strlen;
    char* blocks[50];
    int blockCount;
};

static struct TASK{
    volatile int from;
    volatile int to;
    volatile long long result;
    pthread_t thread;
    int times;
    struct net netstruct;
};

static struct TASK tasks[4];


static int calcCombinations(char* arr1, int arr1length, int* numbers, int numberslength, int times) { // permutations of #

    //printf("      %s ", arr1);
    clock_t begin = clock();

    char buffer[1000];
    int arr1Index = 0;
    for(int i = 0; i < times; i++) {
        strcpy(buffer + arr1Index, arr1);
        arr1Index+= arr1length;
        if(i != times - 1){
            buffer[arr1Index] = '?';
            arr1Index++;
        }
    }
    int buffer2[100];
    for(int i = 0; i < times; i++) {
        for(int j = 0; j < numberslength; j++) {
            buffer2[j+i*numberslength] = numbers[j];
        }
    }




    //checkBufferIndex = 0;
    //int ret = recursion(arr1, arr1length, numbers, numberslength, "");
    int ret = recursion(buffer, arr1Index, buffer2, numberslength*times, "");

    int l = strlen(arr1);
    arr1[l-1] = 0;
    clock_t end = clock();
    printf("      %s          RET = %d                                                  in %d seconds \n", buffer, ret,(int)((end - begin) / CLOCKS_PER_SEC));

    /*
    for(int i = 0; i < checkBufferIndex; i++) {
        for(int k = 0; k < arr1length; k++) {
            if(arr1[k] == '.' && checkBuffer[i][k] == '#') {
                //printf("ERROR %s\n", arr1);
                //exit(999);
            }
        }
        for(int k = 0; k < 100; k++) {
            checkBuffer[i][k] = 0;
        }
    }
*/
    return ret;
}


static int recursion(const char* buffer, int bufferL, int* numbers, int numbersL, char* previousPath) {
    //printf("CALED %s %d %d REMAINING %s\n", previousPath, bufferL, numbersL, buffer);
    if(bufferL < 0) return 0;
    if(numbersL == 0 && bufferL >= 0) {
        for(int i = 0; i < bufferL; i++) {
            if(buffer[i] == '#') return 0;
        }
        printf("FOUND %s\n", previousPath);
        //strcpy(checkBuffer[checkBufferIndex], previousPath);
        //checkBufferIndex++;
        return 1;
    }
    //do some raff calculation if its eve npossible:
    {
        int numbersSum = 0;
        for(int i = 0; i < numbersL; i++) {
            numbersSum += numbers[i];
        }
        int notPointSum = 0;
        int notHashTagSum = 0;
        for(int i = 0; i < bufferL; i++) {
            if(buffer[i] != '.') {
                notPointSum++;
            }
            if(buffer[i] != '#') {
                notHashTagSum++;
            }
        }
        if(notPointSum < numbersSum) {
            return 0;
        }
        if(notHashTagSum + 1 < numbersL) {
            return 0;
        }
    }


    int number = numbers[0];
    numbers = &numbers[1];
    numbersL--;
    int ret = 0;


    char locBuf[1000];
    strcpy(locBuf, previousPath);
    int fixedLen = strlen(previousPath);

    int i = 0;
    for(; i < bufferL; i++) {
        //check where it could fit:
        if(buffer[i] != '.')
        {
            break;
        }
        locBuf[fixedLen] = '.';
        fixedLen++;
    }
    int firstPossibleLocation = i;
    if(buffer[firstPossibleLocation] == '#') {
        // only one possibility
        int offset = firstPossibleLocation + number + 1; // +1: one point is required in between
        if(buffer[offset-1] == '#') {
            return 0;
        }
        for(int i = firstPossibleLocation; i < firstPossibleLocation + number; i++) {
            if(buffer[i] == '.') {
                return 0;
            }
        }
        for(int i = fixedLen; i < fixedLen + number; i++) {
            locBuf[i] = '#';
        }
        locBuf[fixedLen + number] = '.';
        locBuf[fixedLen + number+1] = '\0';
        if(bufferL - offset == -1) {
            offset--;
            locBuf[fixedLen + number] = '\0';
        }
        ret = recursion(buffer + offset, bufferL - offset, numbers, numbersL, locBuf);
    }else{
        // ? found
        for(i = firstPossibleLocation; i < bufferL; i++) {
            int notPossible = 0;
            for(int j = firstPossibleLocation; j < i; j++) {
                if(buffer[j] == '#') {
                    notPossible = 1;
                }
            }
            for(int j = 0; j < number; j++) {
                if(buffer[i + j] == '.' || i+j == bufferL) {
                    notPossible = 1;
                    break;
                }
            }
            if(notPossible == 0) {
                if(buffer[i+number] == '#' && buffer[i+number] != '\n') { // trailing . required
                    notPossible = 1;
                }
            }
            if(notPossible == 0) {
                int offset = i + number + 1;
                int j;
                for(j = fixedLen; j < fixedLen + (i-firstPossibleLocation); j++) {
                    locBuf[j] = '.';
                }
                for(;j <  fixedLen + (i-firstPossibleLocation) + number; j++) {
                    locBuf[j] = '#';
                }
                locBuf[j] = '.';
                locBuf[j+1] = '\0';
                if(bufferL - offset == -1) {
                        offset--;
                        locBuf[j] = '\0';
                }
                ret += recursion(buffer + offset, bufferL - offset, numbers, numbersL, locBuf);
            }
        }
    }
    //printf("RECUSION RETURN called with [%s] %d numbers  ret=%d\n", buffer, numbersL, ret);
    return ret;

}

static int fac(int param) {
    int result = 1;
    while(param != 1) {
        result *= param;
        param--;
    }
    return result;
}

static int recursion2(const char* buffer, int bufferL, int* numbers, int numbersL) {
    //printf("CALED %s %d %d REMAINING %s\n", previousPath, bufferL, numbersL, buffer);
    if(bufferL < 0) return 0;
    if(numbersL == 0 && bufferL >= 0) {
        for(int i = 0; i < bufferL; i++) {
            if(buffer[i] == '#') return 0;
        }
        return 1;
    }
    int checkLater = -1;
    //do some raff calculation if its eve npossible:
    {
        int numbersSum = 0;
        for(int i = 0; i < numbersL; i++) {
            numbersSum += numbers[i];
        }
        int notPointSum = 0;
        int notHashTagSum = 0;
        int questionMarkSum = 0;
        for(int i = 0; i < bufferL; i++) {
            if(buffer[i] != '.') {
                notPointSum++;
            }
            if(buffer[i] != '#') {
                notHashTagSum++;
            }
            if(buffer[i] == 'ß') {
                questionMarkSum++;
            }
        }
        if(notPointSum < numbersSum) {
            return 0;
        }
        if(notHashTagSum + 1 < numbersL) {
            return 0;
        }
        if(questionMarkSum == bufferL) {
            // can be calculated:
            int gaps = bufferL - numbersSum - numbersL  + 1;
            int positions = numbersL + 1;
            return gaps * positions;
        }
    }


    int number = numbers[0];
    numbers = &numbers[1];
    numbersL--;
    int ret = 0;



    int i = 0;
    for(; i < bufferL; i++) {
        //check where it could fit:
        if(buffer[i] != '.')
        {
            break;
        }
    }
    int firstPossibleLocation = i;
    if(buffer[firstPossibleLocation] == '#') {
        // only one possibility
        int offset = firstPossibleLocation + number + 1; // +1: one point is required in between
        if(buffer[offset-1] == '#') {
            return 0;
        }
        for(int i = firstPossibleLocation; i < firstPossibleLocation + number; i++) {
            if(buffer[i] == '.') {
                return 0;
            }
        }
        if(bufferL - offset == -1) {
            offset--;
        }
        ret = recursion2(buffer + offset, bufferL - offset, numbers, numbersL);
    }else{
        // ? found
        for(i = firstPossibleLocation; i < bufferL; i++) {
            int notPossible = 0;
            for(int j = firstPossibleLocation; j < i; j++) {
                if(buffer[j] == '#') {
                    notPossible = 1;
                }
            }
            for(int j = 0; j < number; j++) {
                if(buffer[i + j] == '.' || i+j == bufferL) {
                    notPossible = 1;
                    break;
                }
            }
            if(notPossible == 0) {
                if(buffer[i+number] == '#' && buffer[i+number] != '\n') { // trailing . required
                    notPossible = 1;
                }
            }
            if(notPossible == 0) {
                int offset = i + number + 1;
                int j;
                if(bufferL - offset == -1) {
                        offset--;
                }
                ret += recursion2(buffer + offset, bufferL - offset, numbers, numbersL);
            }
        }
    }
    //printf("RECUSION RETURN called with [%s] %d numbers  ret=%d\n", buffer, numbersL, ret);

    return ret;

}


static void printm(char m[1000][1000]) {
    for(int i = 0; i < rows; i++) {

        printf("%s", m[i]);
        printf("\n");
    }
}









//=============================================================
// part 2:
//=============================================================


static int calcLayer(struct net* net) {
    int layerToWrite = net->layers;
    char* block = net->blocks[layerToWrite-1];

    int currentStateL = 99999;
    int currentStateH = 0;
    for(int i = 0; i <= net->numberslen ; i++) {
        for(int j = 0; j <= net->numberslen ; j++) {
            if(net->adjMatrix[layerToWrite-1][i][j]) {
                if(j < currentStateL) {
                    currentStateL = j;
                }
                if(j>currentStateH) {
                    currentStateH = j;
                }
            }
        }
    }

    int blockLength = 0;
    while(block[blockLength] != '.') blockLength++;
    for(int startstate = currentStateL; startstate <= currentStateH; startstate++) {
        for(int goalstate = startstate; goalstate <= net->numberslen; goalstate++) {
            int result = 0;
            if(startstate == 0 && goalstate == 0) {
                int possible = 1;
                for(int i = 0; i < blockLength; i++) {
                    if(block[i] == '#') {
                        possible = 0;
                        break;
                    }
                }
                if(possible) {
                    result = 1;
                }
            }else{

                result = recursion2(block, blockLength, &net->numbers[startstate + 1], goalstate - startstate );
                //printf("%d | %s (%d)|| %d ... (%d)  = %d\n", net->layers, block, blockLength, net->numbers[startstate + 1], goalstate - startstate, result );

            }
            net->adjMatrix[net->layers][startstate][goalstate] = result;
        }
    }
    net->layers++;


    return 0;
}

void doMultiplication(struct net* net, int layer, int startState, long currentFactor, long long * p) {
    if(startState == net->numberslen && layer == net->layers) {
        //printf("ADDING %d\n", currentFactor);
        *p += currentFactor;
        return;
    }
    for(int i = 0; i  <= net->numberslen; i++) {
        int factor = net->adjMatrix[layer][startState][i];
        if(factor == 0) continue;
        //printf("found factor %d at [%d][%d][%d]\n", factor, layer, startState, i);
        doMultiplication(net, layer+1, i, currentFactor * factor, p);
    }

}

struct net net;

static long long calcCombinations2(char* arr1, int arr1length, int* numbers, int numberslength, int times, struct net* net) {
    for(int i = 0; i < 100; i++) {
        for(int j = 0; j < 100; j++) {
            for(int k = 0; k < 100; k++) {
                net->adjMatrix[i][j][k] = 0;
            }
        }
    }

    int arr1Index = 0;
    for(int i = 0; i < times; i++) {
        strcpy(net->str + arr1Index, arr1);
        arr1Index+= arr1length;
        if(i != times - 1){
            net->str[arr1Index] = '?';
            arr1Index++;
        }
    }
    net->strlen = arr1Index;
    net->str[net->strlen] = '.';// might be good if there is a bug somewhere in my code.
    net->str[net->strlen + 1] = '\0';
    for(int i = 0; i < times; i++) {
        for(int j = 0; j < numberslength; j++) {
            net->numbers[1+j+i*numberslength] = numbers[j];
        }
    }
    net->numbers[0] = 0;// startNumber
    net->numberslen = times * numberslength;

    int startNewBlock = 1;
    net->blockCount = 0;
    for(int i = 0; i < net->strlen; i++) {
        if(startNewBlock && (net->str[i] == '#'|| net->str[i] == '?')) {
            net->blocks[net->blockCount] = &net->str[i];
            net->blockCount ++;
            startNewBlock = 0;
        }
        if(!startNewBlock && net->str[i] == '.') {
            startNewBlock = 1;
        }
    }
    net->adjMatrix[0][0][0] = 1;
    net->layers = 1;

    printf("%d blocks for %s:\n", net->blockCount, net->str);

    while(net->layers <= net->blockCount){
        calcLayer(net);
    }

/*
    for(int i = 0; i < net->layers; i++) {
        printf("\n layer/block %d:\n",i);
        for(int j = 0; j <= net->numberslen; j++) {
            for(int k = 0; k <= net->numberslen; k++) {
                printf("%d",net->adjMatrix[i][j][k]);
            }
            printf("\n");
        }
    }
*/

    long long result = 0;
    doMultiplication(net, 0 , 0, 1, &result);
    printf("result = %lld\n", result);


    return result;
}














static void myThreadFun(void* arg) {
    struct TASK* task = arg;
    long long sum = 0;
    printf("STARTED\n");
    printf("THREAD started from %d to %d with times=%d\n", task->from, task->to, task->times);


    for(int i = task->from; i < task->to; i++) {
        char* line = &matrix[i][0];
        int length = 0;
        while(1) {
            if(line[length] == ' ') {
                break;
            }
            length++;
        }
        int ints[50];
        int intPartCount =0;
        int number = 0;
        int j = length+1;
        while(1) {
            int mychar = line[j];
            j++;
            if(mychar == ',' || mychar == '\n') {
                ints[intPartCount] = number;
                number = 0;
                intPartCount++;
                if(mychar == '\n') {
                    break;
                }
            }else{
                number *= 10;
                number += mychar - '0';
            }

        }
        long long res = calcCombinations2(line, length, ints,intPartCount, task->times, &(task->netstruct));
        sum += res;

      //  int check = calcCombinations(line, length, ints,intPartCount, task->times);

       // if(res != check) {
        //    printf("ERROR FOUND AT %s\n", line);
       // }


        printf("===========%4d %d%%==============\n",i, (i - task->from)*100/(task->to - task->from));
    }

    task->result = sum;
    return 0x0;
}

void day12(int times) {
    read();
    printf("ROWS = %d\n",rows);

    //printm(matrix);

    long long sum = 0;

    int threads = 6;
    int lastEnd = 0;
    for(int i = 0; i < threads; i++) {
        tasks[i].from =lastEnd;
        lastEnd = lastEnd + 1 + rows/threads;
        tasks[i].to = lastEnd;
        if(i == threads-1) {
            tasks[i].to = rows;
        }
        tasks[i].times = times;
        pthread_create(&tasks[i].thread, NULL, &myThreadFun, (void*) &tasks[i]);
    }
    for(int j = 0; j < threads; j++) {
            void* result;
        pthread_join(tasks[j].thread, &result);

    }



    for(int j = 0; j < threads; j++) {

        sum += tasks[j].result;
    }


    printf("SUM = %lld\n", sum); //7716 7146 7108


}



static void read() {
    FILE* inputfile = fopen("./input/input12.txt","r");
    int c;
    int i = 0;
    int j = 0;
    do
    {
        c = (char)fgetc(inputfile);
        matrix[i][j] = c;
        if(c == '\n') {
            i++;
            j = 0;
        }else{
            j++;
        }
    } while(c != EOF);
    rows = i;
    fclose(inputfile);
}

