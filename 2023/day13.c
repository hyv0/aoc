
#include <stdio.h>
#include <stdlib.h>

static struct PROBLEM{
    int matrix[50][50];
    int wi;
    int wj;
};

static void read();

static struct PROBLEM problems[500];
static int problemCount = 0;


static void printm(struct PROBLEM * p) {
    printf("\nMATRIX [%d][%d]\n", p->wi, p->wj);
    for(int i = 0; i < p->wi; i++) {
        for(int j = 0; j < p->wj; j++) {
            printf("%c", p->matrix[i][j]);
        }
        printf("\n");
    }
}


static long long checkRowSymmetry(struct PROBLEM* problem) {
    long long ret = 0;
    for(int i = 0; i < problem -> wi - 1; i++) {
        int foundDiff = 0;
        for(int j = 0; j < problem -> wj; j++) {
            if(problem->matrix[i][j] != problem->matrix[i+1][j]) {
                foundDiff = 1;
                break;
            }
        }
        if(!foundDiff) {
            // two same rows found
            int toLowerBound = i;
            int toHigherBound = problem->wi - i - 2;
            int lowestBound = toLowerBound < toHigherBound ? toLowerBound : toHigherBound;
            printf("possible horizontal axis at %d with span %d\n", i, lowestBound);
            for(int ii = 1; ii <= lowestBound; ii++) {
                for(int j = 0; j < problem->wj;j++) {
                    //printf("COMPARING [%d][%d] with  [%d][%d]\n", i-ii, j, i+1+ii, j);
                    if(problem->matrix[i-ii][j] != problem->matrix[i+1+ii][j]) {
                        foundDiff = 1;
                        break;
                    }

                }
            }
            if(!foundDiff) {
                printf("OK at %d-%d\n", i, i+1);
                ret += 100*(i+1);
            }
        }
    }
    return ret;
}

static long long checkLineSymmetry(struct PROBLEM* problem) {
    long long ret = 0;
    for(int j = 0; j < problem -> wj - 1; j++) {
        int foundDiff = 0;
        for(int i = 0; i < problem -> wi; i++) {
            if(problem->matrix[i][j] != problem->matrix[i][j+1]) {
                foundDiff = 1;
                break;
            }
        }
        if(!foundDiff) {
            // two same rows found
            int toLowerBound = j;
            int toHigherBound = problem->wj - j - 2;
            int lowestBound = toLowerBound < toHigherBound ? toLowerBound : toHigherBound;
            printf("possible vertical axis at %d with span %d\n", j, lowestBound);
            for(int jj = 1; jj <= lowestBound; jj++) {
                for(int i = 0; i < problem->wi;i++) {
                    if(problem->matrix[i][j-jj] != problem->matrix[i][j+jj+1]) {
                        foundDiff = 1;
                        break;
                    }

                }
            }
            if(!foundDiff) {
                printf("OK at %d-%d\n", j, j+1);
                ret += (j+1);
            }
        }
    }
    return ret;
}


 //=============

 static long long checkRowSymmetrySmudge(struct PROBLEM* problem) {
    long long ret = 0;
    for(int i = 0; i < problem -> wi - 1; i++) {
        int foundDiffs = 0;
        for(int j = 0; j < problem -> wj; j++) {
            if(problem->matrix[i][j] != problem->matrix[i+1][j]) {
                foundDiffs += 1;
            }
        }
        if(foundDiffs < 2) {
            // two same rows found
            int toLowerBound = i;
            int toHigherBound = problem->wi - i - 2;
            int lowestBound = toLowerBound < toHigherBound ? toLowerBound : toHigherBound;
            printf("possible horizontal axis at %d with span %d\n", i, lowestBound);
            for(int ii = 1; ii <= lowestBound; ii++) {
                for(int j = 0; j < problem->wj;j++) {
                    //printf("COMPARING [%d][%d] with  [%d][%d]\n", i-ii, j, i+1+ii, j);
                    if(problem->matrix[i-ii][j] != problem->matrix[i+1+ii][j]) {
                        foundDiffs++;
                        if(foundDiffs >= 2)
                            break;
                    }

                }
            }
            if(foundDiffs == 1) {
                printf("OK at %d-%d\n", i, i+1);
                ret += 100*(i+1);
            }
        }
    }
    return ret;
}


static long long checkLineSymmetrySmudge(struct PROBLEM* problem) {
    long long ret = 0;
    for(int j = 0; j < problem -> wj - 1; j++) {
        int foundDiffs = 0;
        for(int i = 0; i < problem -> wi; i++) {
            if(problem->matrix[i][j] != problem->matrix[i][j+1]) {
                foundDiffs ++;
            }
        }
        if(foundDiffs < 2) {
            // two same rows found
            int toLowerBound = j;
            int toHigherBound = problem->wj - j - 2;
            int lowestBound = toLowerBound < toHigherBound ? toLowerBound : toHigherBound;
            printf("possible vertical axis at %d with span %d\n", j, lowestBound);
            for(int jj = 1; jj <= lowestBound; jj++) {
                for(int i = 0; i < problem->wi;i++) {
                    if(problem->matrix[i][j-jj] != problem->matrix[i][j+jj+1]) {
                        foundDiffs ++;
                        if(foundDiffs >= 2)
                            break;
                    }

                }
            }
            if(foundDiffs == 1) {
                printf("OK at %d-%d\n", j, j+1);
                ret += (j+1);
            }
        }
    }
    return ret;
}

void day13(int part) {
    printf("Day 13 (2023)\n");
    read();
    printf("READ input (%d)\n", problemCount);
    long long ret = 0;
    for(int i = 0; i < problemCount; i++ ){
        printm(&problems[i]);
        if(part == 2) {
            ret += checkRowSymmetrySmudge(&problems[i]);
            ret += checkLineSymmetrySmudge(&problems[i]);
        }else{
            ret += checkLineSymmetry(&problems[i]);
            ret += checkRowSymmetry(&problems[i]);
        }

    }
    printf("\n\nSUM = %lld\n", ret); // 38636

}

static void read() {
    FILE* inputfile = fopen("./input/input13.txt","r");
    int c;
    int i = 0;
    int j = 0;
    int lastCharWasLF = 0;
    do
    {
        c = (char)fgetc(inputfile);
        problems[problemCount].matrix[i][j] = c;
        if(c == '\n') {
            if(lastCharWasLF) {
                problems[problemCount].wi = i;
                problemCount++;
                i = -1;
            }else{
                problems[problemCount].wj = j;
            }
            lastCharWasLF = 1;
            i++;
            j = 0;
        }else{
            j++;
            lastCharWasLF = 0;
        }
    } while(c != EOF);
    fclose(inputfile);
}
