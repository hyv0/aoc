#pragma once
#include <cstdint>

class FileParser;

class Solver{

    public:
    Solver(const FileParser& parser);
    Solver& operator=(const FileParser&) = delete;
    int part1(uint64_t& solution);
    int part2(uint64_t& solution);



    int efficientAlgo(uint64_t& solutionPart1, uint64_t& solutionPart2);

    private:
    const FileParser& parser;
    int getDigits(uint64_t nr);

};