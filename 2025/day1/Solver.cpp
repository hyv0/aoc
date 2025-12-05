#include <iostream>
#include "Solver.h"
#include "FileParser.h"
#include "Dial.h"


Solver::Solver(const FileParser& parser) : parser(parser){

}
int Solver::part1(int& solutionPart1){
    Dial dial(100,50);
    solutionPart1 = 0;
    for(const Line& line : parser.getLines()){
        if(line.firstChar == 'L')
        {
            dial.turnLeft(line.number);
        }else if(line.firstChar == 'R'){
            dial.turnRight(line.number);
            
        }else{
            std::cerr << "UNKNOWN OPERATION " << line.firstChar << std::endl;
            return -1;
        }
        if(dial.getCurrentNr() == 0) {
            solutionPart1++;
        }
    }
    return 0;
}


int Solver::part2(int& solutionPart2){
    Dial dial(100,50);
    solutionPart2 = 0;
    for(const Line& line : parser.getLines()){


        int fullTurns = line.number / 100;
        int before = dial.getCurrentNr();
        if(line.firstChar == 'L')
        {
            dial.turnLeft(line.number);
            if(( dial.getCurrentNr() > before || dial.getCurrentNr() == 0) && before != 0) {
                fullTurns++;
            }
        }else if(line.firstChar == 'R'){
            dial.turnRight(line.number);
            if( dial.getCurrentNr() < before) {
                fullTurns++;
            }

            
        }else{
            std::cerr << "UNKNOWN OPERATION " << line.firstChar << std::endl;
            return -1;
        }
        solutionPart2+=fullTurns;
    }
    return 0;
}
