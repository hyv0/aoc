#include <iostream>
#include <chrono>

#include <iomanip>
#include "FileParser.h"
#include "Solver.h"

int main() {
    FileParser parser("./_inputs/day3.txt");
    std::cout << "Reading " << parser.getFileName() << std::endl;
    int result = parser.readFile();
    if(result != 0) {return result;}

    Solver solver(parser);


    const auto timeA = std::chrono::high_resolution_clock::now();

    int solutionPart1 = 0;
    result = solver.part1(solutionPart1);
    if(result != 0) {return result;}

    const auto timeB = std::chrono::high_resolution_clock::now();

    uint64_t solutionPart2 = 0;
    result = solver.part2(solutionPart2);
    if(result != 0) {return result;}

    const auto timeC = std::chrono::high_resolution_clock::now();

    std::cout << "PART 1 = " << std::setfill(' ') << std::setw(15) << solutionPart1 << "   in " << std::chrono::duration_cast<std::chrono::microseconds>(timeB - timeA).count() << "us" <<std::endl;
    std::cout << "PART 2 = " << std::setfill(' ') << std::setw(15) << solutionPart2 << "   in " << std::chrono::duration_cast<std::chrono::microseconds>(timeC - timeB).count() << "us" <<std::endl;
    
    
    return 0;

}
