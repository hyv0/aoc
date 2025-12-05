#include <iostream>
#include "Solver.h"
#include "FileParser.h"


Solver::Solver(const FileParser& parser) : parser(parser){

}

typedef struct {
    int i;
    int j; 
}Pair;

Pair directions[] = {{0,1},{1,1},{1,0},{1,-1},{-1,1},{-1,0},{-1,-1},{0,-1}};

int Solver::part1(int& solutionPart1){
    solutionPart1 = 0;
    
    Map map = parser.getMap();
    int width = map.width;
    int height = map.height;
    
    for(int i = height - 1; i >= 0; i--) {
        for(int j = width - 1; j >= 0 ; j--) {
            int c = map.arr[i][j];
            if(c == '@'){
                int count = 0;
                for(int k = 7; k >= 0; k--) {
                    int ii = i + directions[k].i;
                    int jj = j + directions[k].j;
                    if(ii >= 0 && ii < height && jj >= 0 && jj < width) {
                        int c = map.arr[ii][jj];
                        if(c == '@'){
                            count++;
                        }
                    }
                }
                if(count <= 3) {
                    solutionPart1++;
                }
            }
        }
    }
    
    

    return 0;
}

struct NodeS;
struct NodeS{
    int neighbourCount;
    struct NodeS* neighbours[8];
};

typedef struct NodeS Node;

int Solver::part2(int& solutionPart2){
    Node nodeGrid[150][150] = {0};
    Map map = parser.getMap();
    int width = map.width;
    int height = map.height;
    Node** trunk = new Node*[150 * 150 * 8];
    int trunkSize = 0;
    solutionPart2 = 0;

    // 1. step: build graph:
    for(int i = height - 1; i >= 0; i--) {
        for(int j = width - 1; j >= 0 ; j--) {
            int c = map.arr[i][j];
            if(c == '@'){
                for(int k = 3; k >= 0; k--) { // use directions: right, and all three down directions. 
                    int ii = i + directions[k].i;
                    int jj = j + directions[k].j;
                    if(ii < height && jj >= 0 && jj < width) {
                        int cc = map.arr[ii][jj];
                        if(cc== '@'){
                            nodeGrid[i][j].neighbours[k] = &nodeGrid[ii][jj];
                            nodeGrid[ii][jj].neighbours[7-k] = &nodeGrid[i][j]; // because of the order of the directions we can just calculate 7-k -> adventage: Later when removing neighbours this is better than just having a randomly sorted list
                            nodeGrid[i][j].neighbourCount++;
                            nodeGrid[ii][jj].neighbourCount++;
                        }
                    }
                }
            }
        }
    }

    // 2. step: fill trunk with nodes with less than 4 neighbours
    for(int i = height - 1; i >= 0; i--) {
        for(int j = width - 1; j >= 0 ; j--) {
            if(nodeGrid[i][j].neighbourCount < 4){
                trunk[trunkSize++] = &nodeGrid[i][j];
            }
        }
    }


    // 3. step: remove items from the trunk and the graph
    while(trunkSize != 0) {
        Node* n = trunk[--trunkSize];
        if(n->neighbourCount == 0) {
            continue;
        }
        solutionPart2++;
        for(int i = 7; i>= 0; i--) {
            Node* neighbour = n->neighbours[i];
            if(neighbour == nullptr){
                continue;
            }
            if(neighbour->neighbourCount == 1)
            {
                neighbour->neighbourCount = 0;
                solutionPart2++;
                // n was the last neighbour.
                continue;
            }
            neighbour->neighbours[7 - i] = nullptr;
            neighbour->neighbourCount--;
            if(neighbour->neighbourCount < 4) {
                trunk[trunkSize++] = neighbour;
            }
        }
        n->neighbourCount = 0;
    }

    // 4. step: visualisation:
    /*
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width ; j++) {
            char c = '.';
            
            if(nodeGrid[i][j].neighbourCount != 0) {
                c = '0' + nodeGrid[i][j].neighbourCount;
            }
            std::cout << c;
        }
        std::cout << std::endl;
    }
    */
    
    delete[] trunk;
    return 0;
}
