#include <iostream>
#include "Solver.h"
#include "FileParser.h"


Solver::Solver(const FileParser& parser) : parser(parser){

}


int Solver::part1(int& solutionPart1){
    solutionPart1 = 0;
    for(const Line& line : parser.getLines()){
            
        int highest = -1;
        int secondHighest = -1;
        for(int i = 98;  i >= 0; i--) {
            int digit = line.digits[i] - '0';
            if(digit  >= highest) {
                secondHighest = highest;
                highest = digit;
            }
        }
        //if(secondHighest == -1) {
        //    secondHighest = line.digits[99] - '0';
        //}
        if(secondHighest < line.digits[99] - '0') {
            secondHighest = line.digits[99] - '0';
        }
        solutionPart1 += highest*10+secondHighest;
    }
    return 0;
}
int Solver::part2(uint64_t& solutionPart2){
    solutionPart2 = 0;
    constexpr int NUMBERL = 12;
    for(const Line& line : parser.getLines()){
        uint64_t number = 0;
        int alreadyUsedIndex = -1;
        

        for(int i = NUMBERL; i > 0; i--) {
            int highest = 0;
            int newAlreadyUsedIndex = 0;
            for(int k = 100 - i; k > alreadyUsedIndex; k--){
                int digit = line.digits[k] - '0';
                if(digit >= highest) {
                    highest = digit;
                    newAlreadyUsedIndex = k;
                }
            }
            alreadyUsedIndex = newAlreadyUsedIndex;
            number = number * 10 + highest;
        }
        solutionPart2 += number;


    }
    return 0;
}
