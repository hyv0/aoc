
#include <stdio.h>
#include <stdlib.h>
static char matrixOriginal[200][200];
static char matrix1[200][200];
static char matrix2[200][200];
static char matrixLast10Cycles[200][200];

# define HASHCOUNT 10000000
static int hashes[HASHCOUNT];

static int rows = 0;
static int columns = 0;

static void read() ;

struct POINT{
    int i;
    int j;
};
static struct POINT points1[2100];
static struct POINT points2[2100];
static int pL = 0;

static long calcLoad(char m[200][200]) {
    long load = 0;
    for ( int j = 0; j < columns; j++) {
        int nearestStone = -1;
        int slided = 0;
        for(int i = 0; i < rows; i++) {
            if(m[i][j] == '#') {
                slided = 0;
                nearestStone = i;
            }
            if(m[i][j] == 'O') {
                slided++;
                //load += slided;
                int newPosition = nearestStone + slided ;
                load += rows - newPosition ;
            }
        }
    }
    return load;
}

static void slide(int direction, char src[200][200], char dest[200][200]) {
    if(direction ==0 ) {
        for ( int j = 0; j < columns; j++) {
            int nearestStone = -1;
            int slided = 0;
            for(int i = 0; i < rows; i++) {
                if(src[i][j] == '#') {
                    slided = 0;
                    nearestStone = i;
                    dest[i][j] = '#';
                }else{
                    dest[i][j] = '.';
                }
                if(src[i][j] == 'O') {
                    slided++;
                    //load += slided;
                    int newPosition = nearestStone + slided ;
                    dest[newPosition][j] = 'O';
                }
            }
        }
    }else if(direction ==2 ) {
        for ( int j = 0; j < columns; j++) {
            int nearestStone = rows;
            int slided = 0;
            for(int i = rows-1; i >= 0; i--) {
                if(src[i][j] == '#') {
                    slided = 0;
                    nearestStone = i;
                    dest[i][j] = '#';
                }else{
                    dest[i][j] = '.';
                }
                if(src[i][j] == 'O') {
                    slided++;
                    //load += slided;
                    int newPosition = nearestStone - slided ;
                    dest[newPosition][j] = 'O';
                }
            }
        }
    }else if(direction ==1 ) {
            for ( int i = 0; i < rows; i++) {
                int nearestStone = columns;
                int slided = 0;
                for(int j = columns-1; j >= 0; j--) {
                    if(src[i][j] == '#') {
                        slided = 0;
                        nearestStone = j;
                        dest[i][j] = '#';
                    }else{
                        dest[i][j] = '.';
                    }
                    if(src[i][j] == 'O') {
                        slided++;
                        //load += slided;
                        int newPosition = nearestStone - slided ;
                        dest[i][newPosition] = 'O';
                    }
                }
            }

    }else if(direction ==3 ) {
            for ( int i = 0; i < rows; i++) {
                int nearestStone = -1;
                int slided = 0;
                for(int j =0; j < columns; j++) {
                    if(src[i][j] == '#') {
                        slided = 0;
                        nearestStone = j;
                        dest[i][j] = '#';
                    }else{
                        dest[i][j] = '.';
                    }
                    if(src[i][j] == 'O') {
                        slided++;
                        //load += slided;
                        int newPosition = nearestStone + slided ;
                        dest[i][newPosition] = 'O';
                    }
                }
            }

    }
}

static void print(char m[200][200]) {
    printf("=============================================================================\n");
    for(int i = 0; i < rows; i++ ){
        for(int j = 0; j < columns; j++) {
                printf("%c", m[i][j]);
        }
        printf("\n");
    }
    printf("=============================================================================\n");
}

void day14() {
    read();

    for(int i = 0; i < columns; i++) {
        for(int j = 0; j < rows; j++) {
            if(matrixOriginal[i][j] == 'O') {
                points1[pL].i = i;
                points1[pL].j = j;
                pL++;
            }
        }
    }
    for(int i = 0; i < HASHCOUNT; i++ ){
        hashes[i] = 0;
    }

    printf("ROWS = %d  COUMNS=%d  stones =%d\n",rows, columns, pL);

    printf("load p1 = %ld\n",calcLoad(matrixOriginal));


    for(int i = 0; i < rows; i++ ){
        for(int j = 0; j < columns; j++) {
            matrix1[i][j] = matrixOriginal[i][j];
        }
    }



    #define checkAfter (65)
    long i;
    for(i = 0; i < 1000000000; i++) {
        slide(0, matrix1, matrix2);
        slide(3,matrix2, matrix1);
        slide(2,matrix1, matrix2);
        slide(1,matrix2, matrix1);


        if(0) {
            print(matrix1);

        }

        if(1) {
            // find hash cycles: first find a cycles, then insert the number in checkAfter makro. !!!!!!!!!!!!!!!!!!!!!!!!!!!
            int hash = 0;
            for(int i = 0; i < rows; i++ ){
                int count = 0;
                for(int j = 0; j < columns; j++) {

                    if(matrix1[i][j] == 'O') {
                        hash += i*j;
                        count++;
                    }
                }
                if(i %3)
                hash += count*17;
            }
            hash = hash % HASHCOUNT;
            if(hashes[hash] != 0) {
                printf("POSSIBLE CYCLE AT %d (%d-%d)   (%d)\n", i-hashes[hash],i,hashes[hash], hash);

            }else{
                hashes[hash] = i;
            }
        }


        if(i % checkAfter == checkAfter-1) {

            printf("calculated %ld cycles\n", i);
            int done = 1;
            for(int i = 0; i < rows; i++ ){
                for(int j = 0; j < columns; j++) {
                    if(matrixLast10Cycles[i][j] != matrix1[i][j]) {
                        done = 0;
                        break;
                    }
                }
            }

            if(done && (i < 1000000000 * 0.7) && i > checkAfter) {
                printf("DONE AFTER %ld cycles", i);
                long k;
                for(k = i; k < 1000000000-100; k += checkAfter){}
                i = k;
                printf("JUMPING TO %d\n",k);
            }

            for(int i = 0; i < rows; i++ ){
                for(int j = 0; j < columns; j++) {
                    matrixLast10Cycles[i][j] = matrix1[i][j];
                }
            }
        }
    }





    printf("\n\n\nFINAL STATE:\n");
    print(matrix1);
    long load1 = calcLoad(matrix1);

    // WOW... IN PART2 YOU SHOULD NOT NOT CALC LOAD AFTER SHIFTING UP. NONESENSE. LOAD ON NORTH BEAM, WHEN IT IS TILTED TO SOUTH. SO LOAD ON NORTH = 0. (well it just took me 2 hours to find my "mistake")
    long load2= 0;
    for(int i = 0; i < rows; i++ ){
        for(int j = 0; j < columns; j++) {
            if(matrix1[i][j]=='O') {
                load2 += rows-i;
            }
        }
    }

    printf("LOAD1 = %ld   LOAD2 = %ld  \n  after %ld cycles\n", load1, load2, i); // 113840 to high; 113835 to high


}





static void read() {
    FILE* inputfile = fopen("./input/input14.txt","r");
    int c;
    int i = 0;
    int j = 0;
    do
    {
        c = (char)fgetc(inputfile);
        matrixOriginal[i][j] = c;
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



