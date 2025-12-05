#pragma once

class FileParser;

class Solver{

    public:
    Solver(const FileParser& parser);
    Solver& operator=(const FileParser&) = delete;
    int part1(int& solution);
    int part2(int& solution);

    private:
    const FileParser& parser;

};