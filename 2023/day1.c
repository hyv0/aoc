
#include <stdio.h>
#include <stdlib.h>
static char matrix[1000][1000];
static int rows = 0;

static void read();

static int findFirstNumber(char* arr, int length) {
    char buff[100];
    strcpy(buff, arr);
    buff[length] = '\0';
    char* occurance = buff + 100;
    int number = -1;
    char* tmp = NULL;

    tmp = strstr(buff, "one") ;
    if(tmp != NULL && tmp < occurance) {
        occurance = tmp;
        number = 1;
    }

    tmp = strstr(buff, "two") ;
    if(tmp != NULL && tmp < occurance) {
        occurance = tmp;
        number = 2;
    }


    tmp = strstr(buff, "three") ;
    if(tmp != NULL && tmp < occurance) {
        occurance = tmp;
        number = 3;
    }


    tmp = strstr(buff, "four") ;
    if(tmp != NULL && tmp < occurance) {
        occurance = tmp;
        number = 4;
    }


    tmp = strstr(buff, "five") ;
    if(tmp != NULL && tmp < occurance) {
        occurance = tmp;
        number = 5;
    }


    tmp = strstr(buff, "six") ;
    if(tmp != NULL && tmp < occurance) {
        occurance = tmp;
        number = 6;
    }


    tmp = strstr(buff, "seven") ;
    if(tmp != NULL && tmp < occurance) {
        occurance = tmp;
        number = 7;
    }


    tmp = strstr(buff, "eight") ;
    if(tmp != NULL && tmp < occurance) {
        occurance = tmp;
        number = 8;
    }


    tmp = strstr(buff, "nine") ;
    if(tmp != NULL && tmp < occurance) {
        occurance = tmp;
        number = 9;
    }



    return number;
}

static char *stRstr(const char *s, const char *m)
{
    const char* ptr = s + strlen(s);
    size_t mlen = strlen(m);
    while (ptr-- != s) {  // until ptr reaches the beginning of s
        if (*ptr==*m) {   // single character match (first characters of s and m)?
            if (strncmp(ptr,m,mlen)==0)  // check the remainder
                return ptr;
        }
    }
    return NULL;
}

static int findLastNumber(char* arr) {
    char buff[100];
    strcpy(buff, arr);
    char* occurance = buff;
    int number = -1;
    char* tmp = NULL;

    tmp = stRstr(buff, "one") ;
    if(tmp != NULL && tmp > occurance) {
        occurance = tmp;
        number = 1;
    }

    tmp = stRstr(buff, "two") ;
    if(tmp != NULL && tmp > occurance) {
        occurance = tmp;
        number = 2;
    }


    tmp = stRstr(buff, "three") ;
    if(tmp != NULL && tmp > occurance) {
        occurance = tmp;
        number = 3;
    }


    tmp = stRstr(buff, "four") ;
    if(tmp != NULL && tmp > occurance) {
        occurance = tmp;
        number = 4;
    }


    tmp = stRstr(buff, "five") ;
    if(tmp != NULL && tmp > occurance) {
        occurance = tmp;
        number = 5;
    }


    tmp = stRstr(buff, "six") ;
    if(tmp != NULL && tmp > occurance) {
        occurance = tmp;
        number = 6;
    }


    tmp = stRstr(buff, "seven") ;
    if(tmp != NULL && tmp > occurance) {
        occurance = tmp;
        number = 7;
    }


    tmp = stRstr(buff, "eight") ;
    if(tmp != NULL && tmp > occurance) {
        occurance = tmp;
        number = 8;
    }


    tmp = stRstr(buff, "nine") ;
    if(tmp != NULL && tmp > occurance) {
        occurance = tmp;
        number = 9;
    }



    return number;
}


void day1() {
    read();
    printf("ROWS = %d\n",rows);
    int sum = 0;
    for(int i = 0; i < rows; i++) {
        int j = 0;
        int last = 0;
        int first = -1;
        while(1) {
            char c = matrix[i][j];

            if(c >= '0' && c<= '9'){
                last = c-'0';
                if(first == -1) {
                    first = c-'0';

                }
            }
            if(c == '\n')break;
            j++;
        }
        sum += first * 10 + last;

    }
    printf("SUM = %d\n", sum);



    long sum2 = 0;
    for(int i = 0; i < rows; i++) {
        int j = 0;
        int last = 0;
        int first = -1;
        while(1) {
            char c = matrix[i][j];
            if(c >= '0' && c<= '9'){
                last = c-'0';
                int after = findLastNumber(&matrix[i][j]);

                if(after != -1) {
                    last = after;
                }
                if(first == -1) {
                    first = c-'0';
                    int prior = findFirstNumber(matrix[i], j);
                    if(prior != -1) {
                        first = prior;
                    }

                }
            }
            if(c == '\n')break;
            j++;
        }
        sum2 += first * 10 + last;
        printf("[%4d] %d   %s", i+1,first*10+last, matrix[i]);

    }
    printf("SUM = %ld\n", sum2);
}




static void read() {
    FILE* inputfile = fopen("./input/input1.txt","r");
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

