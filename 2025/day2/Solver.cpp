#include <iostream>
#include <set>
#include "Solver.h"
#include "FileParser.h"


Solver::Solver(const FileParser& parser) : parser(parser){

}


int Solver::part1(uint64_t& solutionPart1){
    solutionPart1 = 0;
    for(const Range& range : parser.getRanges()){

        uint64_t endNr = range.endNr;
        uint64_t currentNr = range.startNr;
        while(currentNr <= endNr) {
            std::string str = std::to_string(currentNr);
            int mid = str.length() / 2;
            std::string firstPart = str.substr(0, mid);
            std::string secondPart = str.substr(mid);
            if(firstPart == secondPart){
                solutionPart1+=currentNr;
            }
            currentNr++;
        }
    }
    return 0;
}


int Solver::part2(uint64_t& solutionPart2){
    solutionPart2 = 0;
    for(const Range& range : parser.getRanges()){

        uint64_t endNr = range.endNr;
        uint64_t currentNr = range.startNr;

        while(currentNr <= endNr) {
            std::string str = std::to_string(currentNr);

            for(unsigned int split = 1; split <= str.length() / 2; split++) {
                std::string comp = str.substr(0,split);
                for(unsigned int  k = split; k < str.length(); k+=split) {
                    std::string laterPart = str.substr(k, split);
                    if(comp != laterPart) {
                        goto nextSplit;
                    }
                }
                solutionPart2+=currentNr;

                break; 

                nextSplit: 
                continue;
            }

            currentNr++;
        }
    }
    return 0;


}

// ===================================================================================

int Solver::getDigits(uint64_t nr){
    uint64_t comp = 1;
    int digits = 0;
    while(nr >= comp) {
        comp *= 10;
        digits++;
    }
    return digits;
}

// the following algorithm I came up with is orders of magnitues more efficient
int Solver::efficientAlgo(uint64_t& solutionPart1, uint64_t& solutionPart2){
    solutionPart1 = 0;
    solutionPart2 = 0;
    std::set<uint64_t> invalidIds;
    
    
    for(const Range& range : parser.getRanges()){
        
        int digitsRealEnd = getDigits(range.endNr);  
        int digitsRealStart = getDigits(range.startNr);  
        
        invalidIds.clear();

        for(int repeatCount = digitsRealEnd; repeatCount > 1; repeatCount--){ // e.g. when the number is 444444 we check for 444|444 (repeatCount=2) and 44|44|44 (repeatCount=3) and so on
            int digitsEnd = digitsRealEnd;
            int digits = digitsRealStart;
            uint64_t endNr = range.endNr;
            uint64_t startNr = range.startNr; 
            
            loopOverDigitCounts:
            int multiplierFront = 1;
            for(int i =  digits / repeatCount; i > 0; i--) {
                multiplierFront*= 10;
            }
            int multiplierBack = 1;
            for(int i = digits; i > digits / repeatCount; i--) {
                multiplierBack*=10;
            }

            bool digitsNotEqual = digitsEnd != digits;
            if(digitsNotEqual) {
                // divide the range in multiple smaller ranges -> loopOverDigitCounts label
                endNr = multiplierFront * multiplierBack;
                endNr--;
            }

            if((digits % repeatCount ) == 0){
                
                uint64_t firstHalf = startNr / multiplierBack;

                uint64_t firstInvalidId = 0;
                uint64_t diffBetweenInvalidIds = 0;
                for(int i = repeatCount; i > 0; i--) {
                    firstInvalidId = firstInvalidId * multiplierFront + firstHalf;
                    diffBetweenInvalidIds = diffBetweenInvalidIds * multiplierFront + 1; // e.g. for the number 40044004 the next number is 40054005
                }

                if(firstInvalidId < startNr) {
                    firstInvalidId += diffBetweenInvalidIds;
                }
                if(firstInvalidId <= endNr) {
                    int invalidIdCount = endNr / diffBetweenInvalidIds - firstInvalidId / diffBetweenInvalidIds + 1;
                    if(repeatCount == 2){
                        // for repeatCount 2 we don't have to calculate all individual ids. we can calculate the sum:
                        int diffCount = (invalidIdCount ) * (invalidIdCount - 1) / 2;
                        uint64_t allInvalidIdsSummed = diffCount * diffBetweenInvalidIds + (invalidIdCount) * firstInvalidId;
                        solutionPart1 += allInvalidIdsSummed;
                    }

                    for(int i = invalidIdCount-1; i >= 0; i--) {
                        invalidIds.insert(firstInvalidId + i * diffBetweenInvalidIds);
                    }
                }
            }

            if(digitsNotEqual) {
                startNr = endNr + 1;
                endNr = range.endNr;
                digits++;
                goto loopOverDigitCounts;
            }

        }

        for(uint64_t invalidid : invalidIds) {
            solutionPart2 += invalidid;
        }
    }
    return 0;

}
