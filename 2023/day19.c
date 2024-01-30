
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#pragma comment(linker, "/STACK:2000000")
#pragma comment(linker, "/HEAP:8000000000")


static void read();

static struct RULE{
    char variable;
    int greater;
    int constant;
    char nextWorkflow[10];
    struct WORKFLOW* next;
    int accept;
    int reject;
    int isDefault;
};

static struct WORKFLOW{
    char name[10];
    struct RULE rules[5];
    struct WORKFLOW* previous[10];
    int previousCount;
    int ruleCount;
};


static struct  RATING{
    char names[4];
    int values[4];
};

static struct DISTINCT_NODE{
    int childCount;
    struct DISTINCT_NODE** childs;
    int from;
    int to;
};

static struct WORKFLOW workflows[1000] = {0};
static int workflowCount = 0;
static struct RATING ratings[1000];
static int ratingCount = 0;

static void linkRules() {

    for(int i = 0; i < workflowCount; i++) {
        for(int j = 0; j < workflows[i].ruleCount; j++) {
            if(workflows[i].rules[j].accept == 0 && workflows[i].rules[j].reject == 0) {
                for(int k = 0; k < workflowCount; k++) {
                    if(strcmp(workflows[i].rules[j].nextWorkflow, workflows[k].name)==0) {
                        workflows[i].rules[j].next = &workflows[k];
                        workflows[k].previous[workflows[k].previousCount++] = &workflows[i];
                        break;
                    }
                }
            }


        }

    }

}

static struct WORKFLOW *start;


static void searchForewards(struct WORKFLOW* w, int params[][2], int result[][4][2], int* resultCount) {
    printf(".");

    int vars[4][2];
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 2; j++) {
            vars[i][j] = params[i][j];
        }
    }
    for(int i = 0; i < w->ruleCount; i++) {
        struct RULE* r = &(w->rules[i]);
        int index;
        switch(r->variable) {
            case 'x' : index = 0; break;
            case 'm' : index = 1; break;
            case 'a' : index = 2; break;
            case 's' : index = 3; break;
        }


        int varsAcceptCase[4][2];
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 2; j++) {
                varsAcceptCase[i][j] = vars[i][j];
            }
        }
        if(r->isDefault == 0) {
            if(r->greater) {
                if(varsAcceptCase[index][0] < r->constant) {
                    varsAcceptCase[index][0] = r->constant + 1;
                }
            }else{
                if(varsAcceptCase[index][1] > r->constant) {
                    varsAcceptCase[index][1] = r->constant - 1;
                }
            }
        }

        if(r->accept == 0 && r->reject == 0){
            searchForewards(r->next,varsAcceptCase,result,resultCount);
        }else if(r->accept != 0){
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 2; j++) {
                    result[(*resultCount)][i][j] = varsAcceptCase[i][j];
                }
            }
            (*resultCount)++;
        }


        if(r->isDefault == 0){
            if(r->greater) {
                if(vars[index][1] > r->constant) {
                    vars[index][1] = r->constant;
                }
            }else{
                if(vars[index][0] < r->constant) {
                    vars[index][0] = r->constant;
                }
            }
        }
    }
}

static void printtree(struct DISTINCT_NODE* n, int depth) {
    printf("%p %d ", n, depth-1);
    for(int i = 1; i < depth; i++) {
        printf("     ");
    }
    printf("%d - %d\n",n->from, n->to);
    for(int i = 0; i < n->childCount; i++) {
        printtree(n->childs[i], depth+1);
    }

}

static struct DISTINCT_NODE* tree_copy(struct DISTINCT_NODE* toCopy) {
    struct DISTINCT_NODE * node = malloc(sizeof(struct DISTINCT_NODE));
    *node = *toCopy;
    if(toCopy->childCount == 0) return node;
    int mallocCount = (node->childCount/10 + 1) * 10;
    if(node->childCount % 10 == 0) {
        mallocCount-=10;
    }
    node->childs = malloc(sizeof(struct DISTINCT_NODE*) * mallocCount);
    for(int i = 0; i < node->childCount; i++) {
        node->childs[i] = tree_copy(toCopy->childs[i]);
    }
    return node;
}

static void tree_insert(struct DISTINCT_NODE* appendHere, int thresholds[][2], int variableIndex) {
    if(variableIndex == 4) return;
    //printf("treeinsert:i=%d  [%d %d] [%d %d] [%d %d] [%d %d]   cutfront=%d cutend=%d\n",variableIndex, thresholds[0][0], thresholds[0][1], thresholds[1][0], thresholds[1][1] ,thresholds[2][0], thresholds[2][1], thresholds[3][0], thresholds[3][1], splitBeginning, splitEnd);

    if(variableIndex == 3) {
        for(int i = 0; i < appendHere->childCount; i++) {
            if(appendHere->childs[i]->to >= thresholds[3][1] && appendHere->childs[i]->from <= thresholds[3][0]) {
                return;
            }
        }
    }else{
        for(int i = 0; i < appendHere->childCount; i++) {
            if(appendHere->childs[i]->to == thresholds[3][1] && appendHere->childs[i]->from == thresholds[3][0]) {
                tree_insert(appendHere->childs[i], thresholds, variableIndex + 1);
                return;
            }
        }
    }
    int from = thresholds[variableIndex][0];
    int to = thresholds[variableIndex][1];
    int indices[4005];
    for(int i = 0; i < 4005; i++){
        indices[i] = -1;
    }
    for(int i = 0; i < appendHere->childCount; i++) {
        struct DISTINCT_NODE* c = appendHere->childs[i];
        for(int k = c->from; k <= c->to; k++) {
            indices[k] = i;
        }
    }

    int splitBeginning = 0;
    int splitEnd = 0;
    if(from != 0) {
        if(indices[from-1] == indices[from] && indices[from] != -1) {
            splitBeginning = 1;
            // copy
            struct DISTINCT_NODE* newNode = tree_copy(appendHere->childs[indices[from]]);
            appendHere->childs[indices[from]]->from = from;
            newNode->to = from-1;
            appendHere->childCount++;
            if(appendHere->childCount%10 == 1){
                appendHere->childs = realloc(appendHere->childs, sizeof(struct DISTINCT_NODE*) * (appendHere->childCount/10+1)*10);
            }
            appendHere->childs[appendHere->childCount-1] = newNode;

        }
    }
    if(to != 4000) {
        if(indices[to+1] == indices[to] && indices[to] != -1) {
            splitEnd = 1;
            // copy
            struct DISTINCT_NODE* newNode = tree_copy(appendHere->childs[indices[to]]);
            appendHere->childs[indices[to]]->to = to;
            newNode->from = to+1;
            appendHere->childCount++;
            if(appendHere->childCount%10 == 1){
                appendHere->childs = realloc(appendHere->childs, sizeof(struct DISTINCT_NODE*) * (appendHere->childCount/10+1)*10);
            }
            appendHere->childs[appendHere->childCount-1] = newNode;
        }
    }


    int previousIndex = indices[from];
    int areaStart = from;
    for(int k = from; k <= to+1; k++) {
        if(k == to+1) {
            indices[k] = previousIndex+1; // fake so that the last section gets finished
        }
        if(previousIndex != indices[k]) {
            if(previousIndex != -1) {

                tree_insert(appendHere->childs[previousIndex], thresholds, variableIndex + 1);
            }else{
                struct DISTINCT_NODE* newNode = malloc(sizeof(struct DISTINCT_NODE));
                newNode->childCount = 0;
                newNode->childs = NULL;
                newNode->to = k-1;
                newNode->from = areaStart;
                appendHere->childCount++;
                if(appendHere->childCount%10 == 1){
                    appendHere->childs = realloc(appendHere->childs, sizeof(struct DISTINCT_NODE*) * (appendHere->childCount/10+1)*10);
                }
                appendHere->childs[appendHere->childCount-1] = newNode;
                tree_insert(newNode, thresholds, variableIndex + 1);
            }
            areaStart = k;
        }

        previousIndex = indices[k];
    }

//    for(int i = 0; i < appendHere->childCount; i++) {
//        printf("test %p\n",appendHere->childs[i]);
//    }

}

static long long tree_product(long long product, struct DISTINCT_NODE* n) {
    if(n->childCount == 0) return product;

    long long sumOfLeaves = 0;
    for(int i = 0; i < n->childCount; i++) {
        long long diff = n->childs[i]->to - n->childs[i]->from + 1;
        sumOfLeaves += tree_product(product * diff, n->childs[i]);
    }
    return sumOfLeaves;
}

void day19(int part) {
    printf("Day 19 (2023)\n");
    read();
    linkRules();
    printf("file read\n");

    for(int i = 0; i < workflowCount; i++) {
        if(strcmp( workflows[i].name, "in")==0) {
            start = &workflows[i];
        }
//        printf("workflow %d: %s %d\n",i, workflows[i].name, workflows[i].ruleCount);
        for(int j = 0; j < workflows[i].ruleCount; j++) {
//            printf("  rule %d %c %d %d. A=%d R=%d next=%s default=%d\n",j,workflows[i].rules[j].variable, workflows[i].rules[j].constant, workflows[i].rules[j].greater, workflows[i].rules[j].accept, workflows[i].rules[j].reject, workflows[i].rules[j].nextWorkflow, workflows[i].rules[j].isDefault);
        }
    }
    for(int i = 0; i < ratingCount; i++) {
//        printf("%c %d %c %d %c %d\n", ratings[i].names[0], ratings[i].values[0],ratings[i].names[1], ratings[i].values[1], ratings[i].names[2], ratings[i].values[2]);
    }

    long sum = 0;
    for(int a = 0; a < ratingCount; a++) {
        struct WORKFLOW *w = start;

        outer:
        for(int r = 0; r < w->ruleCount; r++) {
            if(w->rules[r].isDefault) {
                if(w->rules[r].accept) {
                    goto accept;
                }
                if(w->rules[r].reject) {
                    goto reject;
                }
                w = w->rules[r].next;
                goto outer;
            }else{
                for(int v = 0; v < 4; v++) {
                    if(w->rules[r].variable == ratings[a].names[v]) {
                        if(w->rules[r].greater) {
                            if(ratings[a].values[v] > w->rules[r].constant ) {
                                if(w->rules[r].accept) {
                                    goto accept;
                                }
                                if(w->rules[r].reject) {
                                    goto reject;
                                }
                                w = w->rules[r].next;
                                goto outer;
                            }
                        }else{
                            if(ratings[a].values[v] < w->rules[r].constant ) {
                                if(w->rules[r].accept) {
                                    goto accept;
                                }
                                if(w->rules[r].reject) {
                                    goto reject;
                                }
                                w = w->rules[r].next;
                                goto outer;
                            }
                        }
                    }
                }
            }
        }

            accept:
            for(int i = 0; i < 4 ; i++) {
                sum+= ratings[a].values[i];
            }

            reject:
;


    }

    printf("SUM = %ld\n", sum);

    int vars[4][2];
    for(int i = 0; i < 4; i++) {
        vars[i][0] = 1;
        vars[i][1] = 4000;
    }
    int p2interimResult[1000][4][2];
    int p2interimResultCount = 0;
    searchForewards(start,vars, p2interimResult, &p2interimResultCount);
    printf("combining %d entrys\n",p2interimResultCount);
    for(int i = 0; i < p2interimResultCount; i++) {
        printf("                [%d %d] [%d %d] [%d %d] [%d %d]\n", p2interimResult[i][0][0], p2interimResult[i][0][1], p2interimResult[i][1][0], p2interimResult[i][1][1] ,p2interimResult[i][2][0], p2interimResult[i][2][1], p2interimResult[i][3][0], p2interimResult[i][3][1]);

    }
    long long p2Result = 0;

    struct DISTINCT_NODE rootNode = {0};
    for(int i = 0; i < p2interimResultCount; i++) {
        int invalid = 0;
        for(int k = 0; k < 4; k++) {
            if(p2interimResult[i][k][0] > p2interimResult[i][k][1]) {
                invalid = 1;
            }
            //z *= p2interimResult[i][k][1] - p2interimResult[i][k][0] + 1;
        }
        if(invalid) {
            continue;
        }
        tree_insert(&rootNode, &(p2interimResult[i]),0);
        //printtree(&rootNode,1);
        //getchar();
    }

    // Loop multiply depth first to-from+1
//    printtree(&rootNode,1);
    p2Result = tree_product(1, &rootNode);

    printf("\np2 result = %lld   (example = 167409079868000)\n", p2Result);

    printf("\n done\n");
    while(1){
            getchar();
    }

}

static void read() {
    FILE* inputfile = fopen("./input/input19.txt","r");

    int bufferLength = 255;
    char buffer[bufferLength];

    while(fgets(buffer, bufferLength, inputfile)) {
        if(buffer[0] == '{'){
            int index = 1;
            for(int v = 0; v < 4; v++) {
                ratings[ratingCount].names[v] = buffer[index];
                index+=2;
                int number = 0;
                while(buffer[index] >= '0' && buffer[index] <= '9') {
                    number *= 10;
                    number += buffer[index] - '0';
                    index++;
                }
                index++;
                ratings[ratingCount].values[v] = number;
            }
            ratingCount++;

        }else if( buffer[0] != '\n'){
            int index = 0;
            while(buffer[index] != '{'){
                workflows[workflowCount].name[index] = buffer[index];
                index++;
            }
            workflows[workflowCount].name[index] = '\0';
            index++;
                int ruleIndex = 0;
            do{
               // printf("RULE: %d  %s\n", ruleIndex, & buffer[index]);
                workflows[workflowCount].rules[ruleIndex].variable = buffer[index];
                index++;
                int a = 0;
                workflows[workflowCount].rules[ruleIndex].isDefault=0;
                if(buffer[index] == '>') {
                    workflows[workflowCount].rules[ruleIndex].greater =1;
                }else if(buffer[index] == '<') {
                    workflows[workflowCount].rules[ruleIndex].greater =0;
                }else{
                    workflows[workflowCount].rules[ruleIndex].isDefault=1;
                    workflows[workflowCount].rules[ruleIndex].accept =(workflows[workflowCount].rules[ruleIndex].variable == 'A');
                    workflows[workflowCount].rules[ruleIndex].reject =(workflows[workflowCount].rules[ruleIndex].variable  == 'R');
                    workflows[workflowCount].rules[ruleIndex].nextWorkflow[a++] = workflows[workflowCount].rules[ruleIndex].variable;

                    goto endRule;
                }
                workflows[workflowCount].rules[ruleIndex].greater = (buffer[index] == '>');
                index++;
                workflows[workflowCount].rules[ruleIndex].constant = 0;
                while(buffer[index] >= '0' && buffer[index] <= '9'){
                    workflows[workflowCount].rules[ruleIndex].constant *= 10;
                    workflows[workflowCount].rules[ruleIndex].constant += buffer[index] - '0';
                    index++;
                }
                index++;
                workflows[workflowCount].rules[ruleIndex].accept =(buffer[index] == 'A');
                workflows[workflowCount].rules[ruleIndex].reject =(buffer[index] == 'R');
                endRule:
                while(buffer[index] >= 'a' && buffer[index] <= 'z') {
                    workflows[workflowCount].rules[ruleIndex].nextWorkflow[a++] = buffer[index];
                    index++;
                }
                if(workflows[workflowCount].rules[ruleIndex].accept ||workflows[workflowCount].rules[ruleIndex].reject){
                    index++;
                }
                workflows[workflowCount].rules[ruleIndex].nextWorkflow[a] = '\0';
                ruleIndex++;
            }while(buffer[index++] == ',');

            workflows[workflowCount].ruleCount = ruleIndex;
            workflowCount++;
        }
    }


    fclose(inputfile);
}
