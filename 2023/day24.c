
#include <stdio.h>
#include <stdlib.h>
static void read() ;
static char matrix1[200][200];


static int wj = 0;
static int wi = 0;

static struct VECTOR{
    double x;
    double y;
    double z;
};

static struct INPUT{
    struct VECTOR s;
    struct VECTOR v;
};

static struct INPUT input[400]={0};
static int inputCount = 0;

static int lineEquation2dim(struct INPUT* a, struct INPUT* b, double* xs, double * ys, double* f1, double* f2) {
    double dividedBy = (b->v.x * a->v.y - b->v.y * a->v.x);
    if(dividedBy ==0) {
        return 0;
    }
    double factor2 = (b->s.y * a->v.x - a->s.y * a->v.x + a->v.y * (-b->s.x + a->s.x)) / dividedBy;
    double factor = (b->s.x + factor2 * b->v.x - a->s.x) / a->v.x;
    *xs = b->s.x + factor2 * b->v.x;
    *ys = b->s.y + factor2 * b->v.y;

    *f1 = factor;
    *f2 = factor2;


    return 1;


}

static double rayCutsPlane(struct VECTOR * a, struct VECTOR * b, struct VECTOR * c, struct VECTOR* basisRay, struct VECTOR* directionRay, struct VECTOR* result) {
    // E: a  + f1*b + f2*c


    struct VECTOR normalvector;
    normalvector.x = b->y * c->z - b->z * c->y;
    normalvector.y = b->z * c->x - b->x * c->z;
    normalvector.z = b->x * c->y - b->y * c->x;

    double normalvectorResult = normalvector.x * a->x + normalvector.y * a->y + normalvector.z * a->z;


    double directionMultiplier = (normalvectorResult-normalvector.x * basisRay->x - normalvector.y * basisRay->y - normalvector.z * basisRay->z) / (normalvector.x * directionRay->x + normalvector.y * directionRay->y + normalvector.z * directionRay->z);


    result->x = basisRay->x + directionMultiplier * directionRay->x;

    result->y = basisRay->y + directionMultiplier * directionRay->y;
    result->z = basisRay->z + directionMultiplier * directionRay->z;
    return directionMultiplier;
}

static int isPointOnPlane(struct VECTOR * a, struct VECTOR * b, struct VECTOR * c, struct VECTOR* point) {
    struct VECTOR normalvector;
    normalvector.x = b->y * c->z - b->z * c->y;
    normalvector.y = b->z * c->x - b->x * c->z;
    normalvector.z = b->x * c->y - b->y * c->x;

    double normalvectorResult = normalvector.x * a->x + normalvector.y * a->y + normalvector.z * a->z;


    double isNull = normalvector.x * point->x + normalvector.y * point->y + normalvector.z * point->z - normalvectorResult;
    return isNull < 0.0001 && isNull > -0.0001;
}

void day24() {
    printf("Day 24 (2023) (merry xmas)\n");
    read();

    printf("read %d\n", inputCount);
    for(int i = 0; i < inputCount; i++) {
        //printf("%3d start = %lld %lld %lld    %lld %lld %lld\n",i,input[i].s.x, input[i].s.y, input[i].s.z, input[i].v.x, input[i].v.y, input[i].v.z );
        //printf("%3d start = %lf %lf %lf    %lf %lf %lf\n",i,input[i].s.x, input[i].s.y, input[i].s.z, input[i].v.x, input[i].v.y, input[i].v.z );
    }


    long part1 = 0;

    for(int i = 0; i < inputCount; i++) {
        for(int j = i+1; j < inputCount; j++) {

            double xs = 0;
            double ys = 0;
            double f1 = 0;
            double f2 = 0;
            int ret = lineEquation2dim(&input[i], &input[j], &xs, &ys, &f1, &f2);
        //    printf("POINT = %lf %lf   %lf  %lf\n", xs,ys, f1, f2);
            if(ret != 0 && f1 >= 0 && f2 >= 0 ) {
                if(f1 == 0 || f2 == 0) {
                    printf("EDGECASE. \n");
                }
                //200000000000000 and at most 400000000000000
                if(xs >= 200000000000000 && xs <= 400000000000000) {
                    if(ys >= 200000000000000 && ys <= 400000000000000) {
                        part1++;
                    }
                }
            }

            if(ret == 0) {
                printf("%3d start = %lf %lf %lf    %lf %lf %lf\n",i,input[i].s.x, input[i].s.y, input[i].s.z, input[i].v.x, input[i].v.y, input[i].v.z );
                printf("%3d start = %lf %lf %lf    %lf %lf %lf\n\n\n",j,input[j].s.x, input[j].s.y, input[j].s.z, input[j].v.x, input[j].v.y, input[j].v.z );
            }
        }
    }
    printf("PART 1 = %ld\n\n\n\n", part1); // 20182 // 16502

    int planeIndex1 = 0;
    int planeIndex2 = 0;

    for(int i = 0; i < inputCount; i++) {
        for(int j = i+1; j < inputCount; j++) {
            struct VECTOR pointOnSecondRay;
            pointOnSecondRay.x = input[j].s.x + input[j].v.x;
            pointOnSecondRay.y = input[j].s.y + input[j].v.y;
            pointOnSecondRay.z = input[j].s.z + input[j].v.z;

            struct VECTOR secondDirVec;
            secondDirVec.x = input[j].s.x - input[i].s.x;
            secondDirVec.y = input[j].s.y - input[i].s.y;
            secondDirVec.z = input[j].s.z - input[i].s.z;

            if(isPointOnPlane(&input[i].s, &input[i].v, &secondDirVec, &pointOnSecondRay)) {
                planeIndex1 = i;
                planeIndex2 = j;

            }

        }
    }


    // TODO: no plane can be created by two rays.....
    printf(" taking %d %d as parallel rays\n", planeIndex1, planeIndex2);

    /*
    struct VECTOR c;
    c.x = input[parallel1Index].s.x + input[parallel1Index].v.x;
    c.y = input[parallel1Index].s.y + input[parallel1Index].v.y;
    c.z = input[parallel1Index].s.z + input[parallel1Index].v.z;

    struct VECTOR a = input[parallel1Index].s;
    struct VECTOR b = input[parallel2Index].s;

    struct VECTOR p1;
    double t1 = rayCutsPlane(&a, &b, &c, &input[0].s, &input[0].v, &p1);

    struct VECTOR p2;
    double t2 = rayCutsPlane(&a, &b, &c, &input[5].s, &input[5].v, &p2);

    struct VECTOR start;
    start.x = p1.x - (p2.x - p1.x)/(t2-t1)*t1;
    start.y = p1.y - (p2.y - p1.y)/(t2-t1)*t1;
    start.z = p1.z - (p2.z - p1.z)/(t2-t1)*t1;

    printf("PART 2: %lf %lf %lf    %lf\n", start.x, start.y, start.z,start.x+ start.y+ start.z ); // 1794656749793824400000000000 // 1183608484999363 to high
*/
    struct VECTOR aa;
    aa.x = 1;
    aa.y = 2;
    aa.z = 4;

    struct VECTOR bb;
    bb.x = 1;
    bb.y = 0;
    bb.z = 1;


    struct VECTOR cc;
    cc.x = 0;
    cc.y = 1;
    cc.z = -2;


    struct VECTOR dd;
    dd.x = 1;
    dd.y = 1;
    dd.z = 7;


    struct VECTOR ee;
    ee.x = 2;
    ee.y = 3;
    ee.z = 4;

    struct VECTOR test;
    rayCutsPlane(&aa, &bb, &cc, &dd, &ee, &test);

    printf("RESULT %lf %lf %lf\n", test.x, test.y, test.z);



    //E: a + u*b + m*c




}





static void read() {
    FILE* inputfile = fopen("./input/input24.txt","r");

    int bufferLength = 255;
    char buffer[bufferLength];

    while(fgets(buffer, bufferLength, inputfile)) {
        char* p = &buffer[0];

        for(int i = 0; i < 6; i++) {
            double casted = atof(p);
            p++;
            while(*p != ' ' && i != 5){
                p++;
            }
            if(i==2) {
                p+=2;
            }

            switch(i) {
                case 0: input[inputCount].s.x = casted; break;
                case 1: input[inputCount].s.y = casted; break;
                case 2: input[inputCount].s.z = casted; break;
                case 3: input[inputCount].v.x = casted; break;
                case 4: input[inputCount].v.y = casted; break;
                case 5: input[inputCount].v.z = casted; break;
            }
        }



        inputCount++;
    }


    fclose(inputfile);
}
