
#include <stdio.h>
#include <stdlib.h>
static char matrix[1000][1000];
static char matrixExtra[1000][1000];
static int rows = 0;
static int columns = 0;

static void read();

static struct POINT{
    int i;
    int j;
};

static void printm(char m[1000][1000], int h, int w) {
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            printf("%c", m[i][j]);
        }
        printf("\n");
    }
}


static int isBetween(int val, int th1, int th2) {
    if(th1 > th2) {
        int tmp = th1;
        th1 = th2;
        th2 = tmp;
    }
    return val > th1 && val < th2;
}

void day11() {
    read();
    printf("ROWS = %d  COUMNS=%d\n",rows, columns);

    // INSERT ROWS AND COLUMNS:
    int emptyRows[100];
    int emptyColumns[100];
    int r = 0;
    int c = 0;

    for(int i = 0; i < rows; i++) {
        int empty = 1;
        for(int j = 0; j < columns; j++) {
            if(matrix[i][j] != '.') {
                empty = 0;
            }
        }
        if(empty) {
            emptyRows[r] = i;
            r++;
        }
    }

    for(int j = 0; j < columns; j++) {
        int empty = 1;
        for(int i = 0; i < rows; i++) {
            if(matrix[i][j] != '.') {
                empty = 0;
            }
        }
        if(empty) {
            emptyColumns[c] = j;
            c++;
        }
    }
    emptyColumns[c] = -1;
    emptyRows[r] = -1;

    for(int u = 0; u < r; u++) {
        printf(" r %d\n", emptyRows[u]);
    }
    for(int u = 0; u < c; u++) {
        printf(" c %d\n", emptyColumns[u]);
    }

    int insertedRows = 0;
    int ii = 0;
    for(int i = 0; i < rows; i++) {
        int jj = 0;
        int insertedColumns = 0;
        for(int j = 0; j < columns; j++) {
            matrixExtra[ii][jj] = matrix[i][j];
            jj++;
            if(emptyColumns[insertedColumns] == j) {
                matrixExtra[ii][jj] = ',';
                jj++;
                insertedColumns++;
            }
        }
        ii++;

        if(emptyRows[insertedRows] == i) {
            insertedRows++;
            for(int j = 0; j < columns + c; j++) {
                matrixExtra[ii][j] = ',';
            }
            ii++;
        }
    }
    //printm(matrixExtra,rows + r, columns + c);

    // search galaxies:
    struct POINT points[1000];
    int p = 0;
    for(int i = 0; i < rows +r; i++) {
        for(int j = 0; j < columns + c; j++) {
            if(matrixExtra[i][j]=='#'){
                points[p].i = i;
                points[p].j = j;
                p++;
            }
        }
    }



    //calculate distances:
    long long sum = 0;
    for(int i = 0; i < p; i++) {
        for(int j = i+1; j < p; j++) {
            struct POINT p1 = points[i];
            struct POINT p2 = points[j];
            sum += abs(p1.i - p2.i) + abs(p1.j - p2.j);
        }
    }

    printf("SUm of distances with one expansion: %ld\n",sum);

    sum = 0;
    p = 0;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            if(matrix[i][j]=='#'){
                points[p].i = i;
                points[p].j = j;
                p++;
            }
        }
    }
    for(int i = 0; i < p; i++) {
        for(int j = i+1; j < p; j++) {
            struct POINT p1 = points[i];
            struct POINT p2 = points[j];
            int distance = abs(p1.i - p2.i) + abs(p1.j - p2.j);
            for(int u = 0; u < r; u++) {
                if(isBetween(emptyRows[u], p1.i, p2.i)) {
                        distance+=999999;
                }
            }
            for( int u = 0; u < c; u++) {
                if(isBetween(emptyColumns[u], p1.j, p2.j)) {
                        distance+=999999;
                }
            }
            sum += distance;
        }
    }
    printf("Sum of distances with great expansion: %lld\n",sum);
}




static void read() {
    FILE* inputfile = fopen("./input/input11.txt","r");
    int c;
    int i = 0;
    int j = 0;
    do
    {
        c = (char)fgetc(inputfile);
        matrix[i][j] = c;
        if(c == '\n') {
            columns = j;
            i++;
            j = 0;
        }else{
            j++;
        }
    } while(c != EOF);
    rows = i;
    fclose(inputfile);
}



