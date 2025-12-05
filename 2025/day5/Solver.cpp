#include <iostream>
#include <list>
#include "Solver.h"
#include "FileParser.h"


Solver::Solver(const FileParser& parser) : parser(parser){

}

int Solver::part1(uint64_t& solutionPart1){
    solutionPart1 = 0;
    for(const int64_t&  id: parser.getIDs()){
        for(const Range& range : parser.getRanges()){
            if(id >= range.startNr && id <= range.endNr) {
                solutionPart1++;
                break;
            }
        }
    }
    return 0;
}


bool compareFct(const Range& arg1, const Range& arg2)
{
    int64_t diff = arg2.startNr - arg1.startNr;
    //if(diff != 0) {
        return diff > 0;
    //}else{
    //    return (arg1.endNr - arg2.startNr) > 0;
    //}

}

int Solver::part2(uint64_t& solutionPart2){
    solutionPart2 = 0;

    // step 1: sort the ranges by their startNr
    std::list<Range> rangesList;
    for(const Range& range : parser.getRanges()){
        rangesList.push_back(range);
    }

    rangesList.sort(compareFct);


    // step 2: merge overlapping ranges:
    std::list<Range>::iterator it = rangesList.begin();
    Range* previous = &(*it);
    it++;
    while(it != rangesList.end()) {
        Range& range = *it;

        if(previous->endNr + 1 >= range.startNr) {
            if(previous->endNr <= range.endNr) {

                previous->endNr = range.endNr;
            }
            it = rangesList.erase(it);
            continue;
        }

        previous = &range;
        it++;
    }

    // step 3: calculate result
    for(Range& r : rangesList) {
        solutionPart2+=r.endNr - r.startNr + 1;
    }

    

    return 0;


}
